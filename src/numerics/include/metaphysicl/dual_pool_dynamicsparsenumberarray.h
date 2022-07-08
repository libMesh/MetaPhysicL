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

#ifndef METAPHYSICL_DUAL_POOLDYNAMICSPARSENUMBERARRAY_H
#define METAPHYSICL_DUAL_POOLDYNAMICSPARSENUMBERARRAY_H

#include "metaphysicl/dualnumber.h"
#include "metaphysicl/pool_dynamicsparsenumberarray.h"

namespace MetaPhysicL
{

template <typename T, typename I>
struct DerivativeType<PoolDynamicSparseNumberArray<T, I>>
{
  typedef PoolDynamicSparseNumberArray<typename DerivativeType<T>::type, I> type;
};

template <typename T, typename I>
struct DerivativesType<PoolDynamicSparseNumberArray<T, I>>
{
  typedef PoolDynamicSparseNumberArray<typename DerivativesType<T>::type, I> type;
};

template <typename T, typename I>
inline typename DerivativeType<PoolDynamicSparseNumberArray<T, I>>::type
derivative(const PoolDynamicSparseNumberArray<T, I> & a, unsigned int derivativeindex)
{
  typename DerivativeType<PoolDynamicSparseNumberArray<T, I>>::type returnval;
  *returnval.dsna_ptr = derivative(*a.dsna_ptr, derivativeindex);
  return returnval;
}

template <typename T, typename I>
inline typename DerivativesType<PoolDynamicSparseNumberArray<T, I>>::type
derivatives(const PoolDynamicSparseNumberArray<T, I> & a)
{
  typename DerivativeType<PoolDynamicSparseNumberArray<T, I>>::type returnval;
  *returnval.dsna_ptr = derivatives(*a.dsna_ptr);
  return returnval;
}

template <typename T, typename I, unsigned int derivativeindex>
struct DerivativeOf<PoolDynamicSparseNumberArray<T, I>, derivativeindex>
{
  static typename DerivativeType<PoolDynamicSparseNumberArray<T, I>>::type
  derivative(const PoolDynamicSparseNumberArray<T, I> & a);
};

// DualNumber is subordinate to PoolDynamicSparseNumberArray

#define DualPoolDynamicSparseNumberArray_comparisons(templatename)                                 \
  template <typename T,                                                                            \
            typename T2,                                                                           \
            typename D,                                                                            \
            typename I,                                                                            \
            bool asd,                                                                              \
            bool reverseorder>                                                                     \
  struct templatename<PoolDynamicSparseNumberArray<T2, I>, DualNumber<T, D, asd>, reverseorder>    \
  {                                                                                                \
    typedef PoolDynamicSparseNumberArray<                                                          \
        typename Symmetric##templatename<T2, DualNumber<T, D, asd>, reverseorder>::supertype,      \
        I>                                                                                         \
        supertype;                                                                                 \
  }

DualPoolDynamicSparseNumberArray_comparisons(CompareTypes);
DualPoolDynamicSparseNumberArray_comparisons(PlusType);
DualPoolDynamicSparseNumberArray_comparisons(MinusType);
DualPoolDynamicSparseNumberArray_comparisons(MultipliesType);
DualPoolDynamicSparseNumberArray_comparisons(DividesType);
DualPoolDynamicSparseNumberArray_comparisons(AndType);
DualPoolDynamicSparseNumberArray_comparisons(OrType);

} // namespace MetaPhysicL

#endif // METAPHYSICL_DUAL_POOLDYNAMICSPARSENUMBERARRAY_H
