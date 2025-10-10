//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
// Copyright (C) 2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-

#ifndef METAPHYSICL_DUALSEMIDYNAMICSPARSENUMBERARRAY_DECL_H
#define METAPHYSICL_DUALSEMIDYNAMICSPARSENUMBERARRAY_DECL_H

#include "metaphysicl/dualnumber_decl.h"
#include "metaphysicl/semidynamicsparsenumberarray_decl.h"
#include "metaphysicl/metaphysicl_device.h"

namespace MetaPhysicL
{

template <typename T, typename I, typename N, typename ArrayWrapper>
struct DerivativeType<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>>
{
  typedef SemiDynamicSparseNumberArrayGeneric<typename DerivativeType<T>::type, I, N, typename ArrayWrapper::template rebind<typename DerivativeType<T>::type>::type> type;
};

template <typename T, typename I, typename N, typename ArrayWrapper>
struct DerivativesType<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>>
{
  typedef SemiDynamicSparseNumberArrayGeneric<typename DerivativesType<T>::type, I, N, typename ArrayWrapper::template rebind<typename DerivativesType<T>::type>::type> type;
};

template <typename T, typename I, typename N, typename ArrayWrapper>
METAPHYSICL_INLINE typename DerivativeType<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>>::type
derivative(const SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper> & a, unsigned int derivativeindex);

template <typename T, typename I, typename N, typename ArrayWrapper>
METAPHYSICL_INLINE typename DerivativesType<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>>::type
derivatives(const SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper> & a);

template <typename T, typename I, typename N, typename ArrayWrapper, unsigned int derivativeindex>
struct DerivativeOf<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>, derivativeindex>
{
  static typename DerivativeType<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>>::type
  derivative(const SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper> & a);
};

// For a vector of values a[i] each of which has a defined gradient,
// the divergence is the sum of derivative_wrt_xi(a[i])

// For a tensor of values, we take the divergence with respect to the
// first index.
template <typename T, typename I, typename N, typename ArrayWrapper>
METAPHYSICL_INLINE typename DerivativeType<T>::type divergence(const SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper> & a);

// For a vector of values, the gradient is going to be a tensor
template <typename T, typename I, typename N, typename ArrayWrapper>
METAPHYSICL_INLINE SemiDynamicSparseNumberArrayGeneric<typename T::derivatives_type, I, N, ArrayWrapper>
gradient(const SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper> & a);

// DualNumber is subordinate to SemiDynamicSparseNumberArrayGeneric

#define DualSemiDynamicSparseNumberArrayGeneric_comparisons(templatename)                                 \
  template <typename T,                                                                            \
            typename T2,                                                                           \
            typename D,                                                                            \
            typename I,                                                                            \
            typename N,                                                                            \
            typename ArrayWrapper,                                                                 \
            bool asd,                                                                              \
            bool reverseorder>                                                                     \
  struct templatename<SemiDynamicSparseNumberArrayGeneric<T2, I, N, ArrayWrapper>, DualNumber<T, D, asd>, reverseorder> \
  {                                                                                                \
    typedef SemiDynamicSparseNumberArrayGeneric<                                                   \
        typename Symmetric##templatename<T2, DualNumber<T, D, asd>, reverseorder>::supertype,      \
        I,                                                                                         \
        N,                                                                                         \
        typename ArrayWrapper::template rebind<typename Symmetric##templatename<T2, DualNumber<T, D, asd>, reverseorder>::supertype>::type> \
        supertype;                                                                                 \
  }

DualSemiDynamicSparseNumberArrayGeneric_comparisons(CompareTypes);
DualSemiDynamicSparseNumberArrayGeneric_comparisons(PlusType);
DualSemiDynamicSparseNumberArrayGeneric_comparisons(MinusType);
DualSemiDynamicSparseNumberArrayGeneric_comparisons(MultipliesType);
DualSemiDynamicSparseNumberArrayGeneric_comparisons(DividesType);
DualSemiDynamicSparseNumberArrayGeneric_comparisons(AndType);
DualSemiDynamicSparseNumberArrayGeneric_comparisons(OrType);

} // namespace MetaPhysicL

#endif // METAPHYSICL_DUALSEMIDYNAMICSPARSENUMBERARRAY_DECL_H
