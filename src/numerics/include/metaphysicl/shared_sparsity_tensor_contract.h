//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
//-----------------------------------------------------------------------el-

#ifndef METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT_H
#define METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT_H

// Fused shared-sparsity matrix product C = A * B for DIM x DIM tensors whose entries
// are DualNumber<S, SparseArray, asd> (SparseArray a DynamicSparseNumberBase subclass,
// e.g. DynamicSparseNumberArray or MOOSE's SemiDynamicSparseNumberArray).
//
// C(i,k) = sum_j A(i,j) * B(j,k), with derivatives by the product rule
//   d C(i,k) = sum_j ( A(i,j).value * dB(j,k) + B(j,k).value * dA(i,j) ).
//
// The naive contraction performs O(DIM^3) scalar AD multiplies, each doing its own
// sparsity_union (plus more unions accumulating the sum) -- so O(DIM^3) unions per
// tensor product. When every entry of A shares one derivative pattern and every entry
// of B shares one (the usual case: all components depend on the same element DOFs),
// the union P_A u P_B is identical everywhere. This routine computes it ONCE and then
// does all the arithmetic on arrays aligned to that union -- zero per-entry unions,
// zero temporaries. It falls back to the plain contraction if the precondition does
// not hold, so it is always correct.
//
// Requirements on the tensor types: operator()(i,j) returning the entry (mutable for C,
// readable for A/B). libMesh's RankTwoTensorTempl satisfies this, so its operator* can
// call fused_shared_sparsity_contract<dim>(result, A, B).

#include "metaphysicl/shared_sparsity_tensor_multiply.h" // same_index_pattern + includes

#include <cstddef>
#include <type_traits>
#include <vector>

namespace MetaPhysicL
{

// Plain contraction (reference / fallback). C must be distinct from A and B.
template <unsigned int DIM, typename TensorC, typename TensorA, typename TensorB>
inline void
plain_tensor_contract(TensorC & C, const TensorA & A, const TensorB & B)
{
  typedef typename std::decay<decltype(A(0u, 0u))>::type Entry;
  for (unsigned int i = 0; i < DIM; ++i)
    for (unsigned int k = 0; k < DIM; ++k)
    {
      Entry s = 0;
      for (unsigned int j = 0; j < DIM; ++j)
        s += A(i, j) * B(j, k);
      C(i, k) = s;
    }
}

// Fused shared-sparsity contraction. C must be distinct from A and B.
template <unsigned int DIM, typename TensorC, typename TensorA, typename TensorB>
inline void
fused_shared_sparsity_contract(TensorC & C, const TensorA & A, const TensorB & B)
{
  typedef typename std::decay<decltype(A(0u, 0u))>::type Entry;   // DualNumber<...>
  typedef typename std::decay<decltype(A(0u, 0u).value())>::type S;

  // If derivatives are switched off, values are all that matter: plain path.
  if (!A(0u, 0u).take_derivatives())
  {
    plain_tensor_contract<DIM>(C, A, B);
    return;
  }

  const auto & Pa = A(0u, 0u).derivatives().nude_indices();
  const auto & Pb = B(0u, 0u).derivatives().nude_indices();

  // Precondition: every entry of A shares Pa, every entry of B shares Pb.
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

  // ---- ONE-TIME union of Pa and Pb -> U, plus slot maps (per-thread reusable) ----
  typedef typename std::remove_reference<decltype(Pa)>::type IndexVec;
  typedef typename IndexVec::value_type I;

  const std::size_t na = Pa.size(), nb = Pb.size();
  static thread_local std::vector<I> U;
  static thread_local std::vector<int> a_slot, b_slot; // U-position of Pa[t] / Pb[t]
  U.clear();
  a_slot.clear();
  b_slot.clear();
  {
    std::size_t ia = 0, ib = 0;
    while (ia < na || ib < nb)
    {
      if (ib >= nb || (ia < na && Pa[ia] < Pb[ib]))       { U.push_back(Pa[ia]); ++ia; }
      else if (ia >= na || Pb[ib] < Pa[ia])                { U.push_back(Pb[ib]); ++ib; }
      else                                                 { U.push_back(Pa[ia]); ++ia; ++ib; }
    }
    // Now record, for each Pa[t] / Pb[t], its position in U (both are sorted subsets).
    std::size_t w = 0;
    for (std::size_t t = 0; t < na; ++t) { while (U[w] != Pa[t]) ++w; a_slot.push_back(int(w)); }
    w = 0;
    for (std::size_t t = 0; t < nb; ++t) { while (U[w] != Pb[t]) ++w; b_slot.push_back(int(w)); }
  }
  const std::size_t M = U.size();

  static thread_local std::vector<S> acc; // derivative accumulator for one output entry

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

} // namespace MetaPhysicL

#endif // METAPHYSICL_SHARED_SPARSITY_TENSOR_CONTRACT_H
