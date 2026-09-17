//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
//-----------------------------------------------------------------------el-

#ifndef METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT_H
#define METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT_H

// Fused shared-sparsity tensor operations for DIM x DIM tensors whose entries are
// DualNumber<S, SparseArray, asd> (SparseArray a DynamicSparseNumberBase subclass:
// DynamicSparseNumberArray or MOOSE's SemiDynamicSparseNumberArray). When every entry
// of a tensor shares one derivative sparsity pattern (the usual case: all components
// depend on the same element DOFs), the naive per-scalar-multiply approach recomputes
// that sparsity union once per scalar multiply; these routines compute it ONCE for the
// whole operation and run the arithmetic on aligned arrays.
//
//   fused_shared_sparsity_contract<DIM>(C, A, B)   C = A*B      (matrix product)
//   fused_shared_sparsity_rotate<DIM>(A, R)        A = R*A*R^T  (in place)
//
// Both are SAFE TO CALL FOR ANY entry type: for non-AD entries (e.g. Real) or entries
// whose derivatives are not a dynamic sparse array, they fall back to the plain
// implementation, and at run time they fall back to the plain path if the shared-pattern
// precondition does not hold. So a generic tensor operator (e.g. libMesh
// TypeTensor::operator*) can call them unconditionally.
//
// The tensor types only need operator()(i,j) (mutable for outputs, readable for inputs),
// which libMesh's TypeTensor / RankTwoTensorTempl provide.

// Deliberately lightweight: only the pattern comparator, so consumers like libMesh
// TypeTensor can use these routines without pulling in DualNumber/container headers.
// The DualNumber and sparse-array types are only needed at instantiation, provided by
// the caller's translation unit.
#include "metaphysicl/sparse_pattern_compare.h" // same_index_pattern

#include <cstddef>
#include <type_traits>
#include <vector>

namespace MetaPhysicL
{

// ---- trait: does Entry have a dynamic-sparse derivative array? --------------------
template <typename...>
struct sst_void { typedef void type; };

template <typename T, typename = void>
struct has_sparse_derivatives : std::false_type {};

template <typename T>
struct has_sparse_derivatives<
    T,
    typename sst_void<decltype(std::declval<const T &>().derivatives().nude_indices())>::type>
    : std::true_type {};

// =====================================================================================
// Contraction  C = A * B
// =====================================================================================

// Plain contraction (reference / fallback). C must be distinct from A and B.
// Accumulate into the OUTPUT entry type (the supertype), matching libMesh semantics
// so mixed-type products (e.g. Real * ADReal) are handled correctly.
template <unsigned int DIM, typename TensorC, typename TensorA, typename TensorB>
inline void
plain_tensor_contract(TensorC & C, const TensorA & A, const TensorB & B)
{
  typedef typename std::decay<decltype(C(0u, 0u))>::type Out;
  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int k = 0; k < DIM; ++k)
    {
      Out s = 0;
      for (unsigned int j = 0; j < DIM; ++j)
        s += A(i, j) * B(j, k);
      C(i, k) = s;
    }
}

// Fused implementation (only instantiated for sparse-AD entries).
template <unsigned int DIM, typename TensorC, typename TensorA, typename TensorB>
inline void
contract_sparse_impl(TensorC & C, const TensorA & A, const TensorB & B)
{
  typedef typename std::decay<decltype(A(0u, 0u))>::type Entry;
  typedef typename std::decay<decltype(A(0u, 0u).value())>::type S;

  if (!A(0u, 0u).take_derivatives())
  {
    plain_tensor_contract<DIM>(C, A, B);
    return;
  }

  const auto & Pa = A(0u, 0u).derivatives().nude_indices();
  const auto & Pb = B(0u, 0u).derivatives().nude_indices();

  bool shared = true;
  for (unsigned int i = 0; i < DIM && shared; ++i)
    for (unsigned int j = 0; j < DIM && shared; ++j)
      if (!same_index_pattern(A(i, j).derivatives().nude_indices(), Pa) ||
          !same_index_pattern(B(i, j).derivatives().nude_indices(), Pb))
        shared = false;
  if (!shared)
  {
    plain_tensor_contract<DIM>(C, A, B);
    return;
  }

  typedef typename std::remove_reference<decltype(Pa)>::type IndexVec;
  typedef typename IndexVec::value_type I;

  const std::size_t na = Pa.size(), nb = Pb.size();
  static thread_local std::vector<I> U;
  static thread_local std::vector<int> a_slot, b_slot;
  U.clear(); a_slot.clear(); b_slot.clear();
  {
    std::size_t ia = 0, ib = 0;
    while (ia < na || ib < nb)
    {
      if (ib >= nb || (ia < na && Pa[ia] < Pb[ib])) { U.push_back(Pa[ia]); ++ia; }
      else if (ia >= na || Pb[ib] < Pa[ia])          { U.push_back(Pb[ib]); ++ib; }
      else                                            { U.push_back(Pa[ia]); ++ia; ++ib; }
    }
    std::size_t w = 0;
    for (std::size_t t = 0; t < na; ++t) { while (U[w] != Pa[t]) ++w; a_slot.push_back(int(w)); }
    w = 0;
    for (std::size_t t = 0; t < nb; ++t) { while (U[w] != Pb[t]) ++w; b_slot.push_back(int(w)); }
  }
  const std::size_t M = U.size();
  static thread_local std::vector<S> acc;

  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int k = 0; k < DIM; ++k)
    {
      acc.assign(M, S(0));
      S val = S(0);
      for (unsigned int j = 0; j < DIM; ++j)
      {
        const Entry & a = A(i, j);
        const Entry & b = B(j, k);
        const S av = a.value(), bv = b.value();
        val += av * bv;
        const auto & ad = a.derivatives().nude_data();
        const auto & bd = b.derivatives().nude_data();
        for (std::size_t t = 0; t < nb; ++t) acc[std::size_t(b_slot[t])] += av * bd[t];
        for (std::size_t t = 0; t < na; ++t) acc[std::size_t(a_slot[t])] += bv * ad[t];
      }
      Entry & c = C(i, k);
      c.value() = val;
      auto & cd = c.derivatives();
      cd.resize(M);
      auto & cdata = cd.nude_data();
      auto & cidx = cd.nude_indices();
      for (std::size_t w = 0; w < M; ++w) { cdata[w] = acc[w]; cidx[w] = U[w]; }
    }
}

template <unsigned int DIM, typename TensorC, typename TensorA, typename TensorB>
inline void contract_dispatch(TensorC & C, const TensorA & A, const TensorB & B, std::true_type)
{ contract_sparse_impl<DIM>(C, A, B); }

template <unsigned int DIM, typename TensorC, typename TensorA, typename TensorB>
inline void contract_dispatch(TensorC & C, const TensorA & A, const TensorB & B, std::false_type)
{ plain_tensor_contract<DIM>(C, A, B); }

// Dispatch: fused for sparse-AD entries (both A and B), plain otherwise.
template <unsigned int DIM, typename TensorC, typename TensorA, typename TensorB>
inline void
fused_shared_sparsity_contract(TensorC & C, const TensorA & A, const TensorB & B)
{
  typedef typename std::decay<decltype(A(0u, 0u))>::type EntryA;
  typedef typename std::decay<decltype(B(0u, 0u))>::type EntryB;
  contract_dispatch<DIM>(
      C, A, B,
      std::integral_constant<bool, has_sparse_derivatives<EntryA>::value &&
                                       has_sparse_derivatives<EntryB>::value>{});
}

// =====================================================================================
// Rotation  A = R * A * R^T   (in place)
// =====================================================================================

// Plain rotate (reference / fallback). A'(i,j) = sum_{k,l} R(i,k) R(j,l) A(k,l).
template <unsigned int DIM, typename TensorA, typename TensorR>
inline void
plain_tensor_rotate(TensorA & A, const TensorR & R)
{
  typedef typename std::decay<decltype(A(0u, 0u))>::type Entry;
  Entry temp[DIM * DIM];
  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int j = 0; j < DIM; ++j)
    {
      Entry s = 0;
      for (unsigned int k = 0; k < DIM; ++k)
        for (unsigned int l = 0; l < DIM; ++l)
          s += R(i, k) * R(j, l) * A(k, l);
      temp[i * DIM + j] = s;
    }
  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int j = 0; j < DIM; ++j)
      A(i, j) = temp[i * DIM + j];
}

// Fused implementation (only instantiated for sparse-AD entries). One union over
// P_R u P_A for the whole rotation; product-rule derivative of R(i,k)*R(j,l)*A(k,l).
template <unsigned int DIM, typename TensorA, typename TensorR>
inline void
rotate_sparse_impl(TensorA & A, const TensorR & R)
{
  typedef typename std::decay<decltype(A(0u, 0u))>::type Entry;
  typedef typename std::decay<decltype(A(0u, 0u).value())>::type S;

  if (!A(0u, 0u).take_derivatives())
  {
    plain_tensor_rotate<DIM>(A, R);
    return;
  }

  const auto & Pr = R(0u, 0u).derivatives().nude_indices();
  const auto & Pa = A(0u, 0u).derivatives().nude_indices();

  bool shared = true;
  for (unsigned int i = 0; i < DIM && shared; ++i)
    for (unsigned int j = 0; j < DIM && shared; ++j)
      if (!same_index_pattern(R(i, j).derivatives().nude_indices(), Pr) ||
          !same_index_pattern(A(i, j).derivatives().nude_indices(), Pa))
        shared = false;
  if (!shared)
  {
    plain_tensor_rotate<DIM>(A, R);
    return;
  }

  typedef typename std::remove_reference<decltype(Pa)>::type IndexVec;
  typedef typename IndexVec::value_type I;

  const std::size_t nr = Pr.size(), na = Pa.size();
  static thread_local std::vector<I> U;
  static thread_local std::vector<int> r_slot, a_slot;
  U.clear(); r_slot.clear(); a_slot.clear();
  {
    std::size_t ir = 0, ia = 0;
    while (ir < nr || ia < na)
    {
      if (ia >= na || (ir < nr && Pr[ir] < Pa[ia])) { U.push_back(Pr[ir]); ++ir; }
      else if (ir >= nr || Pa[ia] < Pr[ir])          { U.push_back(Pa[ia]); ++ia; }
      else                                            { U.push_back(Pr[ir]); ++ir; ++ia; }
    }
    std::size_t w = 0;
    for (std::size_t t = 0; t < nr; ++t) { while (U[w] != Pr[t]) ++w; r_slot.push_back(int(w)); }
    w = 0;
    for (std::size_t t = 0; t < na; ++t) { while (U[w] != Pa[t]) ++w; a_slot.push_back(int(w)); }
  }
  const std::size_t M = U.size();

  // Output buffer: A(i,j) depends on all A(k,l), so accumulate into temps then copy back.
  static thread_local std::vector<S> out_val;   // DIM*DIM values
  static thread_local std::vector<S> out_der;    // DIM*DIM * M derivative entries (flat)
  out_val.assign(DIM * DIM, S(0));
  out_der.assign(DIM * DIM * M, S(0));

  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int j = 0; j < DIM; ++j)
    {
      const std::size_t base = (i * DIM + j) * M;
      S val = S(0);
      for (unsigned int k = 0; k < DIM; ++k)
        for (unsigned int l = 0; l < DIM; ++l)
        {
          const Entry & rik = R(i, k);
          const Entry & rjl = R(j, l);
          const Entry & akl = A(k, l);
          const S rv = rik.value(), sv = rjl.value(), av = akl.value();
          val += rv * sv * av;
          const S cA = rv * sv;   // scales dA(k,l)
          const S cRik = sv * av; // scales dR(i,k)
          const S cRjl = rv * av; // scales dR(j,l)
          const auto & ad = akl.derivatives().nude_data();
          const auto & rid = rik.derivatives().nude_data();
          const auto & rjd = rjl.derivatives().nude_data();
          for (std::size_t t = 0; t < na; ++t) out_der[base + std::size_t(a_slot[t])] += cA * ad[t];
          for (std::size_t t = 0; t < nr; ++t) out_der[base + std::size_t(r_slot[t])] += cRik * rid[t];
          for (std::size_t t = 0; t < nr; ++t) out_der[base + std::size_t(r_slot[t])] += cRjl * rjd[t];
        }
      out_val[i * DIM + j] = val;
    }

  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int j = 0; j < DIM; ++j)
    {
      Entry & a = A(i, j);
      a.value() = out_val[i * DIM + j];
      auto & ad = a.derivatives();
      ad.resize(M);
      auto & adata = ad.nude_data();
      auto & aidx = ad.nude_indices();
      const std::size_t base = (i * DIM + j) * M;
      for (std::size_t w = 0; w < M; ++w) { adata[w] = out_der[base + w]; aidx[w] = U[w]; }
    }
}

template <unsigned int DIM, typename TensorA, typename TensorR>
inline void rotate_dispatch(TensorA & A, const TensorR & R, std::true_type)
{ rotate_sparse_impl<DIM>(A, R); }

template <unsigned int DIM, typename TensorA, typename TensorR>
inline void rotate_dispatch(TensorA & A, const TensorR & R, std::false_type)
{ plain_tensor_rotate<DIM>(A, R); }

// Dispatch: fused for sparse-AD entries (both A and R), plain otherwise.
template <unsigned int DIM, typename TensorA, typename TensorR>
inline void
fused_shared_sparsity_rotate(TensorA & A, const TensorR & R)
{
  typedef typename std::decay<decltype(A(0u, 0u))>::type EntryA;
  typedef typename std::decay<decltype(R(0u, 0u))>::type EntryR;
  rotate_dispatch<DIM>(
      A, R,
      std::integral_constant<bool, has_sparse_derivatives<EntryA>::value &&
                                       has_sparse_derivatives<EntryR>::value>{});
}

} // namespace MetaPhysicL

#endif // METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT_H
