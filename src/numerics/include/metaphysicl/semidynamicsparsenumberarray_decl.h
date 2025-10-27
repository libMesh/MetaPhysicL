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

#ifndef METAPHYSICL_SEMIDYNAMICSPARSENUMBERARRAY_DECL_H
#define METAPHYSICL_SEMIDYNAMICSPARSENUMBERARRAY_DECL_H

#include "metaphysicl/metaphysicl_config.h"

#include "metaphysicl/dynamicsparsenumberbase_decl.h"
#include "metaphysicl/dynamic_std_array_wrapper.h"
#include "metaphysicl/dynamic_kokkos_array_wrapper.h"
#include "metaphysicl/testable.h"

namespace MetaPhysicL
{
template <size_t N>
struct NWrapper
{
  static constexpr size_t size = N;
};

template <typename T, typename I, typename N, typename ArrayWrapper>
class SemiDynamicSparseNumberArrayGeneric
    : public DynamicSparseNumberBase<
          ArrayWrapper, typename ArrayWrapper::template rebind<I>::type,
          SemiDynamicSparseNumberArrayGeneric, T, I, N, ArrayWrapper>,
      public safe_bool<
          SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>> {
public:
  template <typename T2, typename I2 = I>
  struct rebind
  {
    typedef SemiDynamicSparseNumberArrayGeneric<T2, I2, N, ArrayWrapper> other;
  };

  SemiDynamicSparseNumberArrayGeneric() = default;

  METAPHYSICL_INLINE SemiDynamicSparseNumberArrayGeneric(const T & val);

  template <typename T2>
  METAPHYSICL_INLINE SemiDynamicSparseNumberArrayGeneric(const T2 & val);

  SemiDynamicSparseNumberArrayGeneric(SemiDynamicSparseNumberArrayGeneric && src) = default;

  SemiDynamicSparseNumberArrayGeneric & operator=(SemiDynamicSparseNumberArrayGeneric && src) = default;

  SemiDynamicSparseNumberArrayGeneric(const SemiDynamicSparseNumberArrayGeneric & src) = default;

  SemiDynamicSparseNumberArrayGeneric & operator=(const SemiDynamicSparseNumberArrayGeneric & src) = default;

  template <typename T2, typename I2, typename ArrayWrapper2>
  METAPHYSICL_INLINE SemiDynamicSparseNumberArrayGeneric(const SemiDynamicSparseNumberArrayGeneric<T2, I2, N, ArrayWrapper2> & src);

  template <typename T2, typename I2, typename ArrayWrapper2>
  METAPHYSICL_INLINE SemiDynamicSparseNumberArrayGeneric(SemiDynamicSparseNumberArrayGeneric<T2, I2, N, ArrayWrapper2> && src);
};

template <typename T, typename I, typename N>
using SemiDynamicSparseNumberArray = SemiDynamicSparseNumberArrayGeneric<T, I, N, DynamicStdArrayWrapper<T, N::size>>;
#ifdef METAPHYSICL_KOKKOS_COMPILATION
template <typename T, typename I, typename N>
using KokkosSemiDynamicSparseNumberArray = SemiDynamicSparseNumberArrayGeneric<T, I, N, DynamicKokkosArrayWrapper<T, N::size>>;
#endif

//
// Non-member functions
//

template <size_t N,
          typename ArrayWrapper,
          unsigned int index1 = 0,
          typename Data1 = void,
          unsigned int index2 = 0,
          typename Data2 = void,
          unsigned int index3 = 0,
          typename Data3 = void,
          unsigned int index4 = 0,
          typename Data4 = void,
          unsigned int index5 = 0,
          typename Data5 = void,
          unsigned int index6 = 0,
          typename Data6 = void,
          unsigned int index7 = 0,
          typename Data7 = void,
          unsigned int index8 = 0,
          typename Data8 = void>
struct SemiDynamicSparseNumberArrayOf
{
  typedef typename SymmetricCompareTypes<
      Data1,
      typename SymmetricCompareTypes<
          Data2,
          typename SymmetricCompareTypes<
              Data3,
              typename SymmetricCompareTypes<
                  Data4,
                  typename SymmetricCompareTypes<
                      Data5,
                      typename SymmetricCompareTypes<
                          Data6,
                          typename SymmetricCompareTypes<Data7, Data8>::supertype>::supertype>::
                      supertype>::supertype>::supertype>::supertype>::supertype supertype;

  typedef SemiDynamicSparseNumberArrayGeneric<supertype, unsigned int, NWrapper<N>, ArrayWrapper> type;
};

DynamicSparseNumberBase_decl_op(SemiDynamicSparseNumberArrayGeneric, +, Plus)       // Union)
DynamicSparseNumberBase_decl_op(SemiDynamicSparseNumberArrayGeneric, -, Minus)      // Union)
DynamicSparseNumberBase_decl_op(SemiDynamicSparseNumberArrayGeneric, *, Multiplies) // Intersection)
DynamicSparseNumberBase_decl_op(SemiDynamicSparseNumberArrayGeneric, /, Divides)    // First)

// CompareTypes, RawType, ValueType specializations

DynamicSparseNumberBase_comparisons(SemiDynamicSparseNumberArrayGeneric, CompareTypes);
DynamicSparseNumberBase_comparisons(SemiDynamicSparseNumberArrayGeneric, PlusType);
DynamicSparseNumberBase_comparisons(SemiDynamicSparseNumberArrayGeneric, MinusType);
DynamicSparseNumberBase_comparisons(SemiDynamicSparseNumberArrayGeneric, MultipliesType);
DynamicSparseNumberBase_comparisons(SemiDynamicSparseNumberArrayGeneric, DividesType);
DynamicSparseNumberBase_comparisons(SemiDynamicSparseNumberArrayGeneric, AndType);
DynamicSparseNumberBase_comparisons(SemiDynamicSparseNumberArrayGeneric, OrType);

template <typename T, typename I, typename N, typename ArrayWrapper>
struct RawType<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>>
{
private:
  typedef typename RawType<T>::value_type NewT;

public:  
  typedef SemiDynamicSparseNumberArrayGeneric<NewT, I, N, typename ArrayWrapper::template rebind<NewT>::type> value_type;

  METAPHYSICL_INLINE static value_type value(const SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper> & a);
};

template <typename T, typename I, typename N, typename ArrayWrapper>
struct ValueType<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>>
{
  typedef typename ValueType<T>::type type;
};

template <typename T, typename I, typename N, typename ArrayWrapper, typename U>
struct ReplaceAlgebraicType<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>, U>
{
private:
  typedef typename ReplaceAlgebraicType<T,U>::type NewT;

public:
  typedef SemiDynamicSparseNumberArrayGeneric<NewT, I, N, typename ArrayWrapper::template rebind<NewT>::type> type;
};


// For backwards compatibility we still allow violating the C++
// standard by putting our partial template specializations into
// namespace std.
#ifdef METAPHYSICL_ENABLE_STD_VIOLATION
} // namespace MetaPhysicL

namespace std {

using MetaPhysicL::SemiDynamicSparseNumberArrayGeneric;
#endif


template <typename T, typename I, typename N, typename ArrayWrapper>
class numeric_limits<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>>
  : public MetaPhysicL::raw_numeric_limits<SemiDynamicSparseNumberArrayGeneric<T, I, N, ArrayWrapper>, T>
{
};

} // namespace std (deprecated) or MetaPhysicL

#endif // METAPHYSICL_SEMIDYNAMICSPARSENUMBERARRAY_DECL_H
