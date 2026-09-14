//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
//-----------------------------------------------------------------------el-

#ifndef METAPHYSICL_SHARED_SPARSITY_TENSOR_MULTIPLY_H
#define METAPHYSICL_SHARED_SPARSITY_TENSOR_MULTIPLY_H

// When a tensor of AD Reals (a NumberArray/NumberVector whose components are
// DualNumber<S, DynamicSparseNumberArray<S,I>>) is multiplied component-wise,
// each component's derivative multiply normally does its own sparsity_union.
// When every component shares one derivative sparsity pattern (the common case,
// e.g. a RankTwoTensor whose components all depend on the same element DOFs),
// that union is identical for every component. This overload of the
// component_multiply_assign customization point (used by
// NumberArray/NumberVector::operator*=) detects the shared pattern and computes
// the union ONCE for the whole tensor, with no per-component temporary.
//
// It is safe by construction: if the shared-pattern precondition does not hold
// it falls back to the ordinary per-component multiply, which is always correct.
//
// Including this header makes `tensor *= tensor` (and thus `tensor * tensor`)
// use the batched path automatically for the AD sparse case; without it,
// operator*= keeps its plain per-component behavior.

#include "metaphysicl/dualnumber.h"
#include "metaphysicl/dualdynamicsparsenumberarray.h"
#include "metaphysicl/semidynamicsparsenumberarray.h"
#include "metaphysicl/numberarray.h"
#include "metaphysicl/numbervector.h"
#include "metaphysicl/sparse_pattern_compare.h" // same_index_pattern

#include <cstddef>
#include <vector>

namespace MetaPhysicL
{

// Generic batched multiply: out[c] *= in[c] for every component, computing the
// derivative sparsity union once for the whole tensor. `Tensor` must provide
// size() and operator[]; its components must be
// DualNumber<S, DynamicSparseNumberArray<S,I>, asd>.
template <typename Tensor>
inline void
fused_shared_sparsity_multiply(Tensor & out, const Tensor & in)
{
  const std::size_t nc = out.size();
  if (nc == 0)
    return;

  // Representative shared patterns (component 0).
  const auto & Pout = out[0].derivatives().nude_indices();
  const auto & Pin = in[0].derivatives().nude_indices();

  // Precondition check: every component must share the representative pattern.
  // Cheap relative to the arithmetic. If violated, fall back to the correct
  // per-component path (no assumption is made).
  bool shared = true;
  for (std::size_t c = 0; c < nc && shared; ++c)
    if (!same_index_pattern(out[c].derivatives().nude_indices(), Pout) ||
        !same_index_pattern(in[c].derivatives().nude_indices(), Pin))
      shared = false;
  if (!shared)
  {
    for (std::size_t c = 0; c < nc; ++c)
      out[c] *= in[c];
    return;
  }

  // ---- ONE-TIME union of Pout and Pin -> U, plus source maps ----
  // srcpos[w] = k  if U[w] == Pout[k]  (old out-deriv slot), else -1
  // inpos[w]  = j  if U[w] == Pin[j]   (in-deriv slot to add), else -1
  typedef typename std::remove_reference<decltype(Pout)>::type IndexVec;
  typedef typename IndexVec::value_type I;

  const std::size_t no = Pout.size(), ni = Pin.size();
  // Per-thread reusable scratch: after warm-up these keep their capacity, so the
  // batched path performs no heap allocation (important for SemiDynamic derivatives,
  // whose storage is otherwise allocation-free). Not reentrant, but this function
  // never recurses. thread_local keeps it safe under threaded assembly.
  static thread_local std::vector<I> U;
  static thread_local std::vector<int> srcpos, inpos;
  U.clear();
  srcpos.clear();
  inpos.clear();

  std::size_t i = 0, j = 0;
  while (i < no || j < ni)
  {
    if (j >= ni || (i < no && Pout[i] < Pin[j]))
    {
      U.push_back(Pout[i]); srcpos.push_back(int(i)); inpos.push_back(-1); ++i;
    }
    else if (i >= no || Pin[j] < Pout[i])
    {
      U.push_back(Pin[j]); srcpos.push_back(-1); inpos.push_back(int(j)); ++j;
    }
    else // index present in both
    {
      U.push_back(Pout[i]); srcpos.push_back(int(i)); inpos.push_back(int(j)); ++i; ++j;
    }
  }
  const std::size_t M = U.size();

  // ---- per component: expand + scale + add (no per-component union) ----
  for (std::size_t c = 0; c < nc; ++c)
  {
    auto & dc = out[c].derivatives();
    const auto & di = in[c].derivatives();

    // Product rule: d(out) = in.value * d(out) + out.value * d(in). out.value()
    // is the OLD value (value update happens after), matching operator*=.
    const auto a = in[c].value();
    const auto b = out[c].value();

    dc.resize(M); // grows to union size; reuses capacity on repeated calls
    auto & data = dc.nude_data();
    auto & idx = dc.nude_indices();
    const auto & idata = di.nude_data();

    // Reverse gather is safe in place: srcpos[w] <= w and is non-decreasing, so
    // each old value at data[srcpos[w]] is still intact when read.
    for (std::size_t w = M; w-- > 0;)
    {
      auto v = a; v = 0 * v; // zero of the value type
      const int k = srcpos[w];
      if (k >= 0)
        v = a * data[std::size_t(k)];
      const int jj = inpos[w];
      if (jj >= 0)
        v += b * idata[std::size_t(jj)];
      data[w] = v;
    }
    for (std::size_t w = 0; w < M; ++w)
      idx[w] = U[w];

    out[c].value() *= in[c].value();
  }
}

// component_multiply_assign overloads (more specialized than the plain-loop
// defaults in numberarray.h / numbervector.h) for AD components whose
// derivative is a DynamicSparseNumberArray.
template <std::size_t N, typename S, typename I, bool asd>
inline void
component_multiply_assign(
    NumberArray<N, DualNumber<S, DynamicSparseNumberArray<S, I>, asd>> & out,
    const NumberArray<N, DualNumber<S, DynamicSparseNumberArray<S, I>, asd>> & in)
{
  fused_shared_sparsity_multiply(out, in);
}

template <std::size_t N, typename S, typename I, bool asd>
inline void
component_multiply_assign(
    NumberVector<N, DualNumber<S, DynamicSparseNumberArray<S, I>, asd>> & out,
    const NumberVector<N, DualNumber<S, DynamicSparseNumberArray<S, I>, asd>> & in)
{
  fused_shared_sparsity_multiply(out, in);
}

// Same, for SemiDynamicSparseNumberArray-backed derivatives (MOOSE's ADReal type).
// Templated on the Generic form so it covers both the std and Kokkos wrappers.
template <std::size_t N, typename S, typename I, typename Nw, typename AW, bool asd>
inline void
component_multiply_assign(
    NumberArray<N, DualNumber<S, SemiDynamicSparseNumberArrayGeneric<S, I, Nw, AW>, asd>> & out,
    const NumberArray<N, DualNumber<S, SemiDynamicSparseNumberArrayGeneric<S, I, Nw, AW>, asd>> & in)
{
  fused_shared_sparsity_multiply(out, in);
}

template <std::size_t N, typename S, typename I, typename Nw, typename AW, bool asd>
inline void
component_multiply_assign(
    NumberVector<N, DualNumber<S, SemiDynamicSparseNumberArrayGeneric<S, I, Nw, AW>, asd>> & out,
    const NumberVector<N, DualNumber<S, SemiDynamicSparseNumberArrayGeneric<S, I, Nw, AW>, asd>> & in)
{
  fused_shared_sparsity_multiply(out, in);
}

} // namespace MetaPhysicL

#endif // METAPHYSICL_SHARED_SPARSITY_TENSOR_MULTIPLY_H
