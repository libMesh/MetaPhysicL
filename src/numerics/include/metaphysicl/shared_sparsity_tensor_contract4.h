//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
//-----------------------------------------------------------------------el-

#ifndef METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT4_H
#define METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT4_H

// Shared-sparsity fused rank-4 tensor products for entries that are
// DualNumber<S, SparseArray, asd> (SparseArray a DynamicSparseNumberBase subclass).
//
//   fused_shared_sparsity_rank4_times_rank2<DIM>(C, A, b)   C_ij   = sum_kl A_ijkl b_kl
//   fused_shared_sparsity_rank4_times_rank4<DIM>(C, A, B)   C_ijkl = sum_pq A_ijpq B_pqkl
//
// Both are (N^2 x N^2) . (N^2) / (N^2 x N^2) contractions over the flat storage. The naive
// versions perform O(N^4) / O(N^6) scalar AD multiplies, each recomputing its own
// sparsity_union. When every entry of each operand shares one derivative sparsity pattern
// (the usual case: all components depend on the same element DOFs), the union P_A u P_B is
// identical everywhere -- these routines compute it ONCE per product and run the arithmetic
// on aligned arrays. They fall back to the plain contraction for non-AD entries and, at run
// time, when the shared-pattern precondition does not hold, so a generic operator can call
// them unconditionally.
//
// Tensor types need operator()(i,j) (rank-2) / operator()(i,j,k,l) (rank-4); libMesh
// RankTwoTensorTempl and MOOSE RankFourTensorTempl provide these.

#include "metaphysicl/shared_sparsity_tensor_contract.h" // has_sparse_derivatives, same_index_pattern

#include <cstddef>
#include <type_traits>
#include <vector>

namespace MetaPhysicL
{

// Build U = P_A u P_B and the slot maps (position in U of each P_A[t] / P_B[t]).
// Returns the union size M. All arrays are per-thread reusable scratch.
template <typename IndexVecA, typename IndexVecB, typename I>
inline std::size_t
build_union_slots(const IndexVecA & Pa, const IndexVecB & Pb,
                  std::vector<I> & U, std::vector<int> & a_slot, std::vector<int> & b_slot)
{
  const std::size_t na = Pa.size(), nb = Pb.size();
  U.clear(); a_slot.clear(); b_slot.clear();
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
  return U.size();
}

// =====================================================================================
// rank4 * rank2 -> rank2     C_ij = sum_kl A_ijkl b_kl
// =====================================================================================

template <unsigned int DIM, typename TensorC, typename Tensor4, typename Tensor2>
inline void
plain_rank4_times_rank2(TensorC & C, const Tensor4 & A, const Tensor2 & b)
{
  typedef typename std::decay<decltype(C(0u, 0u))>::type Out;
  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int j = 0; j < DIM; ++j)
    {
      Out s = 0;
      for (unsigned int k = 0; k < DIM; ++k)
        for (unsigned int l = 0; l < DIM; ++l)
          s += A(i, j, k, l) * b(k, l);
      C(i, j) = s;
    }
}

template <unsigned int DIM, typename TensorC, typename Tensor4, typename Tensor2>
inline void
rank4_times_rank2_sparse_impl(TensorC & C, const Tensor4 & A, const Tensor2 & b)
{
  typedef typename std::decay<decltype(b(0u, 0u).value())>::type S;

  if (!b(0u, 0u).take_derivatives())
  {
    plain_rank4_times_rank2<DIM>(C, A, b);
    return;
  }

  const auto & Pa = A(0u, 0u, 0u, 0u).derivatives().nude_indices();
  const auto & Pb = b(0u, 0u).derivatives().nude_indices();

  bool shared = true;
  for (unsigned int i = 0; i < DIM && shared; ++i)
    for (unsigned int j = 0; j < DIM && shared; ++j)
    {
      if (!same_index_pattern(b(i, j).derivatives().nude_indices(), Pb)) { shared = false; break; }
      for (unsigned int k = 0; k < DIM && shared; ++k)
        for (unsigned int l = 0; l < DIM && shared; ++l)
          if (!same_index_pattern(A(i, j, k, l).derivatives().nude_indices(), Pa))
            shared = false;
    }
  if (!shared)
  {
    plain_rank4_times_rank2<DIM>(C, A, b);
    return;
  }

  typedef typename std::remove_reference<decltype(Pa)>::type IndexVec;
  typedef typename IndexVec::value_type I;
  const std::size_t na = Pa.size(), nb = Pb.size();
  static thread_local std::vector<I> U;
  static thread_local std::vector<int> a_slot, b_slot;
  const std::size_t M = build_union_slots(Pa, Pb, U, a_slot, b_slot);
  static thread_local std::vector<S> acc;

  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int j = 0; j < DIM; ++j)
    {
      acc.assign(M, S(0));
      S val = S(0);
      for (unsigned int k = 0; k < DIM; ++k)
        for (unsigned int l = 0; l < DIM; ++l)
        {
          const auto & a = A(i, j, k, l);
          const auto & bb = b(k, l);
          const S av = a.value(), bv = bb.value();
          val += av * bv;
          const auto & ad = a.derivatives().nude_data();
          const auto & bd = bb.derivatives().nude_data();
          for (std::size_t t = 0; t < na; ++t) acc[std::size_t(a_slot[t])] += bv * ad[t];
          for (std::size_t t = 0; t < nb; ++t) acc[std::size_t(b_slot[t])] += av * bd[t];
        }
      auto & c = C(i, j);
      c.value() = val;
      auto & cd = c.derivatives();
      cd.resize(M);
      auto & cdata = cd.nude_data();
      auto & cidx = cd.nude_indices();
      for (std::size_t w = 0; w < M; ++w) { cdata[w] = acc[w]; cidx[w] = U[w]; }
    }
}

template <unsigned int DIM, typename TensorC, typename Tensor4, typename Tensor2>
inline void r4r2_dispatch(TensorC & C, const Tensor4 & A, const Tensor2 & b, std::true_type)
{ rank4_times_rank2_sparse_impl<DIM>(C, A, b); }

template <unsigned int DIM, typename TensorC, typename Tensor4, typename Tensor2>
inline void r4r2_dispatch(TensorC & C, const Tensor4 & A, const Tensor2 & b, std::false_type)
{ plain_rank4_times_rank2<DIM>(C, A, b); }

template <unsigned int DIM, typename TensorC, typename Tensor4, typename Tensor2>
inline void
fused_shared_sparsity_rank4_times_rank2(TensorC & C, const Tensor4 & A, const Tensor2 & b)
{
  typedef typename std::decay<decltype(A(0u, 0u, 0u, 0u))>::type EntryA;
  typedef typename std::decay<decltype(b(0u, 0u))>::type EntryB;
  r4r2_dispatch<DIM>(
      C, A, b,
      std::integral_constant<bool, has_sparse_derivatives<EntryA>::value &&
                                       has_sparse_derivatives<EntryB>::value>{});
}

// =====================================================================================
// rank4 * rank4 -> rank4     C_ijkl = sum_pq A_ijpq B_pqkl
// =====================================================================================

template <unsigned int DIM, typename TensorC, typename Tensor4A, typename Tensor4B>
inline void
plain_rank4_times_rank4(TensorC & C, const Tensor4A & A, const Tensor4B & B)
{
  typedef typename std::decay<decltype(C(0u, 0u, 0u, 0u))>::type Out;
  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int j = 0; j < DIM; ++j)
      for (unsigned int k = 0; k < DIM; ++k)
        for (unsigned int l = 0; l < DIM; ++l)
        {
          Out s = 0;
          for (unsigned int p = 0; p < DIM; ++p)
            for (unsigned int q = 0; q < DIM; ++q)
              s += A(i, j, p, q) * B(p, q, k, l);
          C(i, j, k, l) = s;
        }
}

template <unsigned int DIM, typename TensorC, typename Tensor4A, typename Tensor4B>
inline void
rank4_times_rank4_sparse_impl(TensorC & C, const Tensor4A & A, const Tensor4B & B)
{
  typedef typename std::decay<decltype(A(0u, 0u, 0u, 0u).value())>::type S;

  if (!A(0u, 0u, 0u, 0u).take_derivatives())
  {
    plain_rank4_times_rank4<DIM>(C, A, B);
    return;
  }

  const auto & Pa = A(0u, 0u, 0u, 0u).derivatives().nude_indices();
  const auto & Pb = B(0u, 0u, 0u, 0u).derivatives().nude_indices();

  bool shared = true;
  for (unsigned int i = 0; i < DIM && shared; ++i)
    for (unsigned int j = 0; j < DIM && shared; ++j)
      for (unsigned int k = 0; k < DIM && shared; ++k)
        for (unsigned int l = 0; l < DIM && shared; ++l)
          if (!same_index_pattern(A(i, j, k, l).derivatives().nude_indices(), Pa) ||
              !same_index_pattern(B(i, j, k, l).derivatives().nude_indices(), Pb))
            shared = false;
  if (!shared)
  {
    plain_rank4_times_rank4<DIM>(C, A, B);
    return;
  }

  typedef typename std::remove_reference<decltype(Pa)>::type IndexVec;
  typedef typename IndexVec::value_type I;
  const std::size_t na = Pa.size(), nb = Pb.size();
  static thread_local std::vector<I> U;
  static thread_local std::vector<int> a_slot, b_slot;
  const std::size_t M = build_union_slots(Pa, Pb, U, a_slot, b_slot);
  static thread_local std::vector<S> acc;

  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int j = 0; j < DIM; ++j)
      for (unsigned int k = 0; k < DIM; ++k)
        for (unsigned int l = 0; l < DIM; ++l)
        {
          acc.assign(M, S(0));
          S val = S(0);
          for (unsigned int p = 0; p < DIM; ++p)
            for (unsigned int q = 0; q < DIM; ++q)
            {
              const auto & a = A(i, j, p, q);
              const auto & bb = B(p, q, k, l);
              const S av = a.value(), bv = bb.value();
              val += av * bv;
              const auto & ad = a.derivatives().nude_data();
              const auto & bd = bb.derivatives().nude_data();
              for (std::size_t t = 0; t < na; ++t) acc[std::size_t(a_slot[t])] += bv * ad[t];
              for (std::size_t t = 0; t < nb; ++t) acc[std::size_t(b_slot[t])] += av * bd[t];
            }
          auto & c = C(i, j, k, l);
          c.value() = val;
          auto & cd = c.derivatives();
          cd.resize(M);
          auto & cdata = cd.nude_data();
          auto & cidx = cd.nude_indices();
          for (std::size_t w = 0; w < M; ++w) { cdata[w] = acc[w]; cidx[w] = U[w]; }
        }
}

template <unsigned int DIM, typename TensorC, typename Tensor4A, typename Tensor4B>
inline void r4r4_dispatch(TensorC & C, const Tensor4A & A, const Tensor4B & B, std::true_type)
{ rank4_times_rank4_sparse_impl<DIM>(C, A, B); }

template <unsigned int DIM, typename TensorC, typename Tensor4A, typename Tensor4B>
inline void r4r4_dispatch(TensorC & C, const Tensor4A & A, const Tensor4B & B, std::false_type)
{ plain_rank4_times_rank4<DIM>(C, A, B); }

template <unsigned int DIM, typename TensorC, typename Tensor4A, typename Tensor4B>
inline void
fused_shared_sparsity_rank4_times_rank4(TensorC & C, const Tensor4A & A, const Tensor4B & B)
{
  typedef typename std::decay<decltype(A(0u, 0u, 0u, 0u))>::type EntryA;
  typedef typename std::decay<decltype(B(0u, 0u, 0u, 0u))>::type EntryB;
  r4r4_dispatch<DIM>(
      C, A, B,
      std::integral_constant<bool, has_sparse_derivatives<EntryA>::value &&
                                       has_sparse_derivatives<EntryB>::value>{});
}

} // namespace MetaPhysicL

#endif // METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT4_H
