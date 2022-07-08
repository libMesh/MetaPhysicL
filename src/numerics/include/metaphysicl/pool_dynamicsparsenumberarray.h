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
//
// $Id: core.h 37197 2013-02-21 05:49:09Z roystgnr $
//
//--------------------------------------------------------------------------

#ifndef METAPHYSICL_POOL_DYNAMICSPARSENUMBERARRAY_H
#define METAPHYSICL_POOL_DYNAMICSPARSENUMBERARRAY_H

#include "metaphysicl/pool_dynamicsparsenumberarray_decl.h"
#include "metaphysicl/dynamicsparsenumberarray.h"

namespace MetaPhysicL
{
template <typename T, typename I>
void
PoolDynamicSparseNumberArray<T, I>::clear()
{
  dsna_ptr->resize(0);
}

template <typename T, typename I>
PoolDynamicSparseNumberArray<T, I>::PoolDynamicSparseNumberArray()
    : dsna_ptr(getPool<T, I>().acquire())
{
  clear();
}

template <typename T, typename I>
PoolDynamicSparseNumberArray<T, I>::PoolDynamicSparseNumberArray(const T & val)
    : dsna_ptr(getPool<T, I>().acquire())
{
  // Avoid unused variable warnings in opt mode.
  (void)val;
  // This makes no sense unless val is 0!
#ifndef NDEBUG
  if (val)
    throw std::domain_error("Cannot initialize PoolDynamicSparseNumberArray with non-zero scalar");
#endif
  clear();
}

template <typename T, typename I>
template <typename T2>
PoolDynamicSparseNumberArray<T, I>::PoolDynamicSparseNumberArray(const T2 & val)
    : dsna_ptr(getPool<T, I>().acquire())
{
  // Avoid unused variable warnings in opt mode.
  (void)val;
  // This makes no sense unless val is 0!
#ifndef NDEBUG
  if (val)
    throw std::domain_error("Cannot initialize PoolDynamicSparseNumberArray with non-zero scalar");
#endif
  clear();
}

template <typename T, typename I>
template <typename T2, typename I2>
PoolDynamicSparseNumberArray<T, I>::PoolDynamicSparseNumberArray(
    const PoolDynamicSparseNumberArray<T2, I2> & src)
    : dsna_ptr(getPool<T, I>().acquire())
{
  *dsna_ptr = *src.dsna_ptr;
}

template <typename T, typename I>
PoolDynamicSparseNumberArray<T, I>::PoolDynamicSparseNumberArray(
    const PoolDynamicSparseNumberArray<T, I> & src)
    : dsna_ptr(getPool<T, I>().acquire())
{
  *dsna_ptr = *src.dsna_ptr;
}

#ifdef METAPHYSICL_USE_STD_MOVE
template <typename T, typename I>
template <typename T2, typename I2>
PoolDynamicSparseNumberArray<T, I>::PoolDynamicSparseNumberArray(
    PoolDynamicSparseNumberArray<T2, I2> && src)
    : dsna_ptr(getPool<T, I>().acquire())
{
  *dsna_ptr = std::move(*src.dsna_ptr);
}
#endif

template <typename T, typename I>
template <typename T2, typename I2>
inline PoolDynamicSparseNumberArray<typename DotType<T, T2>::supertype,
                                    typename CompareTypes<I, I2>::supertype>
PoolDynamicSparseNumberArray<T, I>::dot(const PoolDynamicSparseNumberArray<T2, I2> & /*a*/) const
{
  typedef typename DotType<T, T2>::supertype TS;
  typedef typename CompareTypes<I, I2>::supertype IS;

  PoolDynamicSparseNumberArray<TS, IS> returnval;

  // FIXME
  metaphysicl_not_implemented();

  return returnval;
}

template <typename T, typename I>
template <typename T2, typename I2>
inline PoolDynamicSparseNumberArray<typename OuterProductType<T, T2>::supertype,
                                    typename CompareTypes<I, I2>::supertype>
PoolDynamicSparseNumberArray<T, I>::outerproduct(
    const PoolDynamicSparseNumberArray<T2, I2> & /*a*/) const
{
  typedef typename OuterProductType<T, T2>::supertype TS;
  typedef typename CompareTypes<I, I2>::supertype IS;
  PoolDynamicSparseNumberArray<TS, IS> returnval;

  // FIXME
  metaphysicl_not_implemented();

  return returnval;
}

//
// Non-member functions
//

template <typename T, typename I, typename I2>
inline PoolDynamicSparseNumberArray<PoolDynamicSparseNumberArray<T, I>, I2>
transpose(const PoolDynamicSparseNumberArray<PoolDynamicSparseNumberArray<T, I2>, I> & /*a*/)
{
  PoolDynamicSparseNumberArray<PoolDynamicSparseNumberArray<T, I>, I2> returnval;

  metaphysicl_not_implemented();

  return returnval;
}

template <typename T, typename I>
inline PoolDynamicSparseNumberArray<typename SumType<T>::supertype, I>
sum(const PoolDynamicSparseNumberArray<T, I> & a)
{
  std::size_t index_size = a.size();

  PoolDynamicSparseNumberArray<typename SumType<T>::supertype, I> returnval;
  returnval.resize(index_size);

  for (unsigned int i = 0; i != index_size; ++i)
  {
    returnval.raw_at(i) = a.raw_at(i).sum();
    returnval.raw_index(i) = a.raw_index(i);
  }

  return returnval;
}

template <typename T, typename I>
inline typename RawType<PoolDynamicSparseNumberArray<T, I>>::value_type
RawType<PoolDynamicSparseNumberArray<T, I>>::value(const PoolDynamicSparseNumberArray<T, I> & a)
{
  value_type returnval;
  returnval.nude_indices() = a.nude_indices();

  std::size_t index_size = a.size();
  returnval.nude_data().resize(index_size);

  for (unsigned int i = 0; i != index_size; ++i)
    returnval.raw_at(i) = RawType<T>::value(a.raw_at(i));
  return returnval;
}

template <typename T, typename I>
inline std::size_t
PoolDynamicSparseNumberArray<T, I>::size() const
{
  return dsna_ptr->size();
}

template <typename T, typename I>
inline void
PoolDynamicSparseNumberArray<T, I>::resize(std::size_t s)
{
  dsna_ptr->resize(s);
}

template <typename T, typename I>
inline T *
PoolDynamicSparseNumberArray<T, I>::raw_data()
{
  return dsna_ptr->raw_data();
}

template <typename T, typename I>
inline const T *
PoolDynamicSparseNumberArray<T, I>::raw_data() const
{
  return dsna_ptr->raw_data();
}

template <typename T, typename I>
inline T &
PoolDynamicSparseNumberArray<T, I>::raw_at(unsigned int i)
{
  return dsna_ptr->raw_at(i);
}

template <typename T, typename I>
inline const T &
PoolDynamicSparseNumberArray<T, I>::raw_at(unsigned int i) const
{
  return dsna_ptr->raw_at(i);
}

template <typename T, typename I>
inline I &
PoolDynamicSparseNumberArray<T, I>::raw_index(unsigned int i)
{
  return dsna_ptr->raw_index(i);
}

template <typename T, typename I>
inline const I &
PoolDynamicSparseNumberArray<T, I>::raw_index(unsigned int i) const
{
  return dsna_ptr->raw_index(i);
}

template <typename T, typename I>
inline const std::vector<T> &
PoolDynamicSparseNumberArray<T, I>::nude_data() const
{
  return dsna_ptr->nude_data();
}

template <typename T, typename I>
inline std::vector<T> &
PoolDynamicSparseNumberArray<T, I>::nude_data()
{
  return dsna_ptr->nude_data();
}

template <typename T, typename I>
inline const std::vector<I> &
PoolDynamicSparseNumberArray<T, I>::nude_indices() const
{
  return dsna_ptr->nude_indices();
}

template <typename T, typename I>
inline std::vector<I> &
PoolDynamicSparseNumberArray<T, I>::nude_indices()
{
  return dsna_ptr->nude_indices();
}

template <typename T, typename I>
inline std::size_t
PoolDynamicSparseNumberArray<T, I>::runtime_index_query(index_value_type i) const
{
  return dsna_ptr->runtime_index_query(i);
}

template <typename T, typename I>
inline std::size_t
PoolDynamicSparseNumberArray<T, I>::runtime_index_of(index_value_type i) const
{
  return dsna_ptr->runtime_index_of(i);
}

template <typename T, typename I>
inline T &
PoolDynamicSparseNumberArray<T, I>::operator[](index_value_type i)
{
  return (*dsna_ptr)[i];
}

template <typename T, typename I>
inline const T &
PoolDynamicSparseNumberArray<T, I>::operator[](index_value_type i) const
{
  return (*dsna_ptr)[i];
}

template <typename T, typename I>
template <unsigned int i>
inline typename PoolDynamicSparseNumberArray<T, I>::template entry_type<i>::type &
PoolDynamicSparseNumberArray<T, I>::get()
{
  return dsna_ptr->template get<i>();
}

template <typename T, typename I>
template <unsigned int i>
inline const typename PoolDynamicSparseNumberArray<T, I>::template entry_type<i>::type &
PoolDynamicSparseNumberArray<T, I>::get() const
{
  return dsna_ptr->template get<i>();
}

template <typename T, typename I>
inline T &
PoolDynamicSparseNumberArray<T, I>::insert(unsigned int i)
{
  return dsna_ptr->insert(i);
}

template <typename T, typename I>
template <unsigned int i>
inline typename PoolDynamicSparseNumberArray<T, I>::template entry_type<i>::type &
PoolDynamicSparseNumberArray<T, I>::insert()
{
  return this->insert(i);
}

template <typename T, typename I>
template <unsigned int i, typename T2>
inline void
PoolDynamicSparseNumberArray<T, I>::set(const T2 & val)
{
  return dsna_ptr->set<i>(val);
}

template <typename T, typename I>
inline bool
PoolDynamicSparseNumberArray<T, I>::boolean_test() const
{
  return dsna_ptr->boolean_test();
}

template <typename T, typename I>
inline PoolDynamicSparseNumberArray<T, I>
PoolDynamicSparseNumberArray<T, I>::operator-() const
{
  PoolDynamicSparseNumberArray<T, I> ret;
  *ret.dsna_ptr = -(*dsna_ptr);
  return ret;
}

// Since this is a dynamically allocated sparsity pattern, we can
// increase it as needed to support e.g. operator+=
template <typename T, typename I>
template <typename Indices2>
inline void
PoolDynamicSparseNumberArray<T, I>::sparsity_union(const Indices2 & new_indices)
{
  dsna_ptr->sparsity_union(new_indices);
}

// Since this is a dynamically allocated sparsity pattern, we can
// decrease it when possible for efficiency
template <typename T, typename I>
template <typename Indices2>
inline void
PoolDynamicSparseNumberArray<T, I>::sparsity_intersection(const Indices2 & new_indices)
{
  dsna_ptr->sparsity_intersection(new_indices);
}

// Since this is a dynamically allocated sparsity pattern, we can
// decrease it when possible for efficiency
template <typename T, typename I>
inline void
PoolDynamicSparseNumberArray<T, I>::sparsity_trim(const value_type tolerance)
{
  dsna_ptr->sparsity_trim(tolerance);
}

// Not defineable since !0 != 0
// SubType<SubTypeArgs...> operator! () const;

template <typename T, typename I>
template <typename T2, typename I2>
inline PoolDynamicSparseNumberArray<T, I> &
PoolDynamicSparseNumberArray<T, I>::operator+=(const PoolDynamicSparseNumberArray<T2, I2> & a)
{
  *dsna_ptr += *a.dsna_ptr;
  return *this;
}

template <typename T, typename I>
template <typename T2, typename I2>
inline PoolDynamicSparseNumberArray<T, I> &
PoolDynamicSparseNumberArray<T, I>::operator-=(const PoolDynamicSparseNumberArray<T2, I2> & a)
{
  *dsna_ptr -= *a.dsna_ptr;
  return *this;
}

template <typename T, typename I>
template <typename T2, typename I2>
inline PoolDynamicSparseNumberArray<T, I> &
PoolDynamicSparseNumberArray<T, I>::operator*=(const PoolDynamicSparseNumberArray<T2, I2> & a)
{
  *dsna_ptr *= *a.dsna_ptr;
  return *this;
}

template <typename T, typename I>
template <typename T2, typename I2>
inline PoolDynamicSparseNumberArray<T, I> &
PoolDynamicSparseNumberArray<T, I>::operator/=(const PoolDynamicSparseNumberArray<T2, I2> & a)
{
  *dsna_ptr /= *a.dsna_ptr;
  return *this;
}

template <typename T, typename I>
template <typename T2>
inline PoolDynamicSparseNumberArray<T, I> &
PoolDynamicSparseNumberArray<T, I>::operator*=(const T2 & a)
{
  *dsna_ptr *= a;
  return *this;
}

template <typename T, typename I>
template <typename T2>
inline PoolDynamicSparseNumberArray<T, I> &
PoolDynamicSparseNumberArray<T, I>::operator/=(const T2 & a)
{
  *dsna_ptr /= a;
  return *this;
}

//
// Non-member functions
//

template <typename BoolT, typename BoolI, typename T, typename I, typename T2, typename I2>
inline typename PoolDynamicSparseNumberArray<T, I>::template rebind<
    typename CompareTypes<T, T2>::supertype,
    typename CompareTypes<I, I2>::supertype>::other
if_else(const PoolDynamicSparseNumberArray<BoolT, BoolI> & condition,
        const PoolDynamicSparseNumberArray<T, I> & if_true,
        const PoolDynamicSparseNumberArray<T2, I2> & if_false)
{
  typedef typename PoolDynamicSparseNumberArray<T, I>::template rebind<
      typename CompareTypes<T, T2>::supertype,
      typename CompareTypes<I, I2>::supertype>::other ReturnType;
  ReturnType returnval;
  *returnval.dsna_ptr = if_else(*condition.dsna_ptr, *if_true.dsna_ptr, *if_false.dsna_ptr);
  return returnval;
}

#define PoolDynamicSparseNumberArray_op_ab(opname, functorname)                                    \
  template <typename T, typename I, typename T2, typename I2>                                      \
  inline typename Symmetric##functorname##Type<PoolDynamicSparseNumberArray<T, I>,                 \
                                               PoolDynamicSparseNumberArray<T2, I2>>::supertype    \
  operator opname(const PoolDynamicSparseNumberArray<T, I> & a,                                    \
                  const PoolDynamicSparseNumberArray<T2, I2> & b)                                  \
  {                                                                                                \
    typedef typename Symmetric##functorname##Type<PoolDynamicSparseNumberArray<T, I>,              \
                                                  PoolDynamicSparseNumberArray<T2, I2>>::supertype \
        type;                                                                                      \
    type returnval = a;                                                                            \
    returnval opname## = b;                                                                        \
    return returnval;                                                                              \
  }

#ifdef METAPHYSICL_USE_STD_MOVE

#define PoolDynamicSparseNumberArray_op(opname, functorname)                                       \
  PoolDynamicSparseNumberArray_op_ab(opname, functorname)                                          \
                                                                                                   \
  template <typename T, typename I, typename T2, typename I2>                                      \
  inline                                                                                           \
  typename Symmetric##functorname##Type<PoolDynamicSparseNumberArray<T, I>,                        \
                                        PoolDynamicSparseNumberArray<T2, I2>>::supertype           \
  operator opname(PoolDynamicSparseNumberArray<T, I> && a,                                         \
                  const PoolDynamicSparseNumberArray<T2, I2> & b)                                  \
  {                                                                                                \
    typedef typename Symmetric##functorname##Type<PoolDynamicSparseNumberArray<T, I>,              \
                                                  PoolDynamicSparseNumberArray<T2, I2>>::supertype \
        type;                                                                                      \
    type returnval = std::move(a);                                                                 \
    returnval opname## = b;                                                                        \
    return returnval;                                                                              \
  }

#else

#define PoolDynamicSparseNumberArray_op(opname, functorname)                                       \
  PoolDynamicSparseNumberArray_op_ab(opname, functorname)

#endif

PoolDynamicSparseNumberArray_op(+, Plus)       // Union)
PoolDynamicSparseNumberArray_op(-, Minus)      // Union)
PoolDynamicSparseNumberArray_op(*, Multiplies) // Intersection)
PoolDynamicSparseNumberArray_op(/, Divides)    // First)

// Let's also allow scalar times vector.
// Scalar plus vector, etc. remain undefined in the sparse context.

template <typename T2, typename I, typename T>
inline typename MultipliesType<PoolDynamicSparseNumberArray<T2, I>, T, true>::supertype
operator*(const T & a, const PoolDynamicSparseNumberArray<T2, I> & b)
{
  typename MultipliesType<PoolDynamicSparseNumberArray<T2, I>, T, true>::supertype returnval;
  *returnval.dsna_ptr = a * (*b.dsna_ptr);
  return returnval;
}

template <typename T2, typename I, typename T>
inline typename MultipliesType<PoolDynamicSparseNumberArray<T2, I>, T>::supertype
operator*(const PoolDynamicSparseNumberArray<T2, I> & a, const T & b)
{
  typename MultipliesType<PoolDynamicSparseNumberArray<T2, I>, T>::supertype returnval;
  *returnval.dsna_ptr = *a.dsna_ptr * b;
  return returnval;
}

template <typename T2, typename I, typename T>
inline typename DividesType<PoolDynamicSparseNumberArray<T2, I>, T>::supertype
operator/(const PoolDynamicSparseNumberArray<T2, I> & a, const T & b)
{
  typename DividesType<PoolDynamicSparseNumberArray<T2, I>, T>::supertype returnval;
  *returnval.dsna_ptr = *a.dsna_ptr / b;
  return returnval;
}

#if __cplusplus >= 201103L
template <typename T2, typename I, typename T>
inline typename MultipliesType<PoolDynamicSparseNumberArray<T2, I>, T>::supertype
operator*(PoolDynamicSparseNumberArray<T2, I> && a, const T & b)
{
  typename MultipliesType<PoolDynamicSparseNumberArray<T2, I>, T>::supertype returnval =
      std::move(a);
  returnval *= b;
  return returnval;
}

template <typename T2, typename I, typename T>
inline typename DividesType<PoolDynamicSparseNumberArray<T2, I>, T>::supertype
operator/(PoolDynamicSparseNumberArray<T2, I> && a, const T & b)
{
  typename DividesType<PoolDynamicSparseNumberArray<T2, I>, T>::supertype returnval =
      std::move(a);

  returnval /= b;

  return returnval;
}
#endif

#define PoolDynamicSparseNumberArray_operator_binary(opname, functorname)                          \
  template <typename T, typename I, typename T2, typename I2>                                      \
  inline typename PoolDynamicSparseNumberArray<T, I>::                                             \
      template rebind<bool, typename CompareTypes<I, I2>::supertype>::other                        \
      operator opname(const PoolDynamicSparseNumberArray<T, I> & a,                                \
                      const PoolDynamicSparseNumberArray<T2, I2> & b)                              \
  {                                                                                                \
                                                                                                   \
    typename PoolDynamicSparseNumberArray<T, I>::                                                  \
        template rebind<bool, typename CompareTypes<I, I2>::supertype>::other returnval;           \
    *returnval.dsna_ptr = (*a.dsna_ptr)opname(*b.dsna_ptr);                                        \
    return returnval;                                                                              \
  }                                                                                                \
  template <typename T2, typename I, typename T>                                                   \
  inline typename PoolDynamicSparseNumberArray<T2, I>::template rebind<bool>::other                \
  operator opname(const PoolDynamicSparseNumberArray<T2, I> & a, const T & b)                      \
  {                                                                                                \
    typename PoolDynamicSparseNumberArray<T2, I>::template rebind<bool>::other returnval;          \
    *returnval.dsna_ptr = (*a.dsna_ptr)opname b;                                                   \
    return returnval;                                                                              \
  }                                                                                                \
  template <typename T2, typename I, typename T>                                                   \
  inline typename PoolDynamicSparseNumberArray<T2, I>::template rebind<bool>::other                \
  operator opname(const T & a, const PoolDynamicSparseNumberArray<T2, I> & b)                      \
  {                                                                                                \
    typename PoolDynamicSparseNumberArray<T2, I>::template rebind<bool>::other returnval;          \
                                                                                                   \
    *returnval.dsna_ptr = a opname(*b.dsna_ptr);                                                   \
    return returnval;                                                                              \
  }

// NOTE: unary functions for which 0-op-0 is true are undefined compile-time
// errors, because there's no efficient way to have them make sense in
// the sparse context.

PoolDynamicSparseNumberArray_operator_binary(<, less)
// PoolDynamicSparseNumberArray_operator_binary(<=)
PoolDynamicSparseNumberArray_operator_binary(>, greater)
// PoolDynamicSparseNumberArray_operator_binary(>=)
// PoolDynamicSparseNumberArray_operator_binary(==)
PoolDynamicSparseNumberArray_operator_binary(!=, not_equal_to)

// FIXME - make && an intersection rather than a union for efficiency
PoolDynamicSparseNumberArray_operator_binary(&&, logical_and)
PoolDynamicSparseNumberArray_operator_binary(||, logical_or)

template <typename T, typename I>
inline std::ostream &
operator<<(std::ostream & output, const PoolDynamicSparseNumberArray<T, I> & a)
{
  output << *a.dsna_ptr;
  return output;
}

} // namespace MetaPhysicL

namespace std
{

using MetaPhysicL::CompareTypes;
using MetaPhysicL::PoolDynamicSparseNumberArray;
using MetaPhysicL::SymmetricCompareTypes;

#define PoolDynamicSparseNumberArray_std_unary(funcname)                                           \
  template <typename T, typename I>                                                                \
  inline PoolDynamicSparseNumberArray<T, I> funcname(const PoolDynamicSparseNumberArray<T, I> & a) \
  {                                                                                                \
    PoolDynamicSparseNumberArray<T, I> returnval;                                                  \
    *returnval.dsna_ptr = funcname(*a.dsna_ptr);                                                   \
    return returnval;                                                                              \
  }

#define PoolDynamicSparseNumberArray_fl_unary(funcname)                                            \
  PoolDynamicSparseNumberArray_std_unary(funcname##f)                                              \
      PoolDynamicSparseNumberArray_std_unary(funcname##l)

#define PoolDynamicSparseNumberArray_stdfl_unary(funcname)                                         \
  PoolDynamicSparseNumberArray_std_unary(funcname) PoolDynamicSparseNumberArray_fl_unary(funcname)

#define PoolDynamicSparseNumberArray_std_binary_union(funcname)                                    \
  template <typename T, typename I, typename T2, typename I2>                                      \
  inline typename PoolDynamicSparseNumberArray<T, I>::template rebind<                             \
      typename SymmetricCompareTypes<T, T2>::supertype,                                            \
      typename CompareTypes<I, I2>::supertype>::other                                              \
  funcname(const PoolDynamicSparseNumberArray<T, I> & a,                                           \
           const PoolDynamicSparseNumberArray<T2, I2> & b)                                         \
  {                                                                                                \
    typename PoolDynamicSparseNumberArray<T, I>::template rebind<                                  \
        typename SymmetricCompareTypes<T, T2>::supertype,                                          \
        typename CompareTypes<I, I2>::supertype>::other returnval;                                 \
    *returnval.dsna_ptr = funcname(*a.dsna_ptr, *b.dsna_ptr);                                      \
    return returnval;                                                                              \
  }                                                                                                \
                                                                                                   \
  template <typename T, typename I, typename T2>                                                   \
  inline typename PoolDynamicSparseNumberArray<T, I>::                                             \
      template rebind<typename SymmetricCompareTypes<T, T2>::supertype, I>::other                  \
      funcname(const PoolDynamicSparseNumberArray<T, I> & a, const T2 & b)                         \
  {                                                                                                \
    typename PoolDynamicSparseNumberArray<T, I>::                                                  \
        template rebind<typename SymmetricCompareTypes<T, T2>::supertype, I>::other returnval;     \
    *returnval.dsna_ptr = funcname(*a.dsna_ptr, b);                                                \
    return returnval;                                                                              \
  }                                                                                                \
                                                                                                   \
  template <typename T2, typename I, typename T>                                                   \
  inline typename PoolDynamicSparseNumberArray<T2, I>::                                            \
      template rebind<typename SymmetricCompareTypes<T, T2>::supertype, I>::other                  \
      funcname(const T & a, const PoolDynamicSparseNumberArray<T2, I> & b)                         \
  {                                                                                                \
    typename PoolDynamicSparseNumberArray<T2, I>::                                                 \
        template rebind<typename SymmetricCompareTypes<T, T2>::supertype, I>::other returnval;     \
    *returnval.dsna_ptr = funcname(a, *b.dsna_ptr);                                                \
    return returnval;                                                                              \
  }

#define PoolDynamicSparseNumberArray_fl_binary_union(funcname)                                     \
  PoolDynamicSparseNumberArray_std_binary_union(funcname##f)                                       \
  PoolDynamicSparseNumberArray_std_binary_union(funcname##l)

#define PoolDynamicSparseNumberArray_stdfl_binary_union(funcname)                                  \
  PoolDynamicSparseNumberArray_std_binary_union(funcname)                                          \
  PoolDynamicSparseNumberArray_fl_binary_union(funcname)

// Pow needs its own specialization, both to avoid being confused by
// pow<T1,T2> and because pow(x,0) isn't 0.
template <typename T, typename I, typename T2>
inline typename PoolDynamicSparseNumberArray<T, I>::
    template rebind<typename SymmetricCompareTypes<T, T2>::supertype, I>::other
    pow(const PoolDynamicSparseNumberArray<T, I> & a, const T2 & b)
{
  typename PoolDynamicSparseNumberArray<T, I>::
      template rebind<typename SymmetricCompareTypes<T, T2>::supertype, I>::other returnval;
  *returnval.dsna_ptr = pow(*a.dsna_ptr, b);
  return returnval;
}

// NOTE: unary functions for which f(0) != 0 are undefined compile-time
// errors, because there's no efficient way to have them make sense in
// the sparse context.

// PoolDynamicSparseNumberArray_std_binary(pow) // separate definition
// PoolDynamicSparseNumberArray_std_unary(exp)
// PoolDynamicSparseNumberArray_std_unary(log)
// PoolDynamicSparseNumberArray_std_unary(log10)
PoolDynamicSparseNumberArray_std_unary(sin)
// PoolDynamicSparseNumberArray_std_unary(cos)
PoolDynamicSparseNumberArray_std_unary(tan)
PoolDynamicSparseNumberArray_std_unary(asin)
// PoolDynamicSparseNumberArray_std_unary(acos)
PoolDynamicSparseNumberArray_std_unary(atan)
PoolDynamicSparseNumberArray_std_binary_union(atan2)
PoolDynamicSparseNumberArray_std_unary(sinh)
// PoolDynamicSparseNumberArray_std_unary(cosh)
PoolDynamicSparseNumberArray_std_unary(tanh)
PoolDynamicSparseNumberArray_std_unary(sqrt)
PoolDynamicSparseNumberArray_std_unary(abs)
PoolDynamicSparseNumberArray_std_unary(fabs)
PoolDynamicSparseNumberArray_std_binary_union(max)
PoolDynamicSparseNumberArray_std_binary_union(min)
PoolDynamicSparseNumberArray_std_unary(ceil)
PoolDynamicSparseNumberArray_std_unary(floor)
PoolDynamicSparseNumberArray_std_binary_union(fmod) // TODO: optimize this

#if __cplusplus >= 201103L
PoolDynamicSparseNumberArray_std_unary(llabs)
PoolDynamicSparseNumberArray_std_unary(imaxabs)
PoolDynamicSparseNumberArray_fl_unary(fabs)
PoolDynamicSparseNumberArray_stdfl_unary(expm1)
PoolDynamicSparseNumberArray_fl_unary(sqrt)
PoolDynamicSparseNumberArray_stdfl_unary(cbrt)
PoolDynamicSparseNumberArray_fl_unary(sin)
PoolDynamicSparseNumberArray_fl_unary(tan)
PoolDynamicSparseNumberArray_fl_unary(asin)
PoolDynamicSparseNumberArray_fl_unary(atan)
PoolDynamicSparseNumberArray_stdfl_unary(asinh)
PoolDynamicSparseNumberArray_stdfl_unary(atanh)
PoolDynamicSparseNumberArray_stdfl_unary(erf)
PoolDynamicSparseNumberArray_fl_unary(ceil)
PoolDynamicSparseNumberArray_fl_unary(floor)
PoolDynamicSparseNumberArray_stdfl_unary(trunc)
PoolDynamicSparseNumberArray_stdfl_unary(round)
PoolDynamicSparseNumberArray_stdfl_unary(nearbyint)
PoolDynamicSparseNumberArray_stdfl_unary(rint)

PoolDynamicSparseNumberArray_fl_binary_union(fmod)
PoolDynamicSparseNumberArray_stdfl_binary_union(remainder) // TODO: optimize this
PoolDynamicSparseNumberArray_stdfl_binary_union(fmax)
PoolDynamicSparseNumberArray_stdfl_binary_union(fmin)
PoolDynamicSparseNumberArray_stdfl_binary_union(fdim)
PoolDynamicSparseNumberArray_stdfl_binary_union(hypot)
PoolDynamicSparseNumberArray_fl_binary_union(atan2)
#endif // __cplusplus >= 201103L

#define PoolDynamicSparseNumberArray_std_unary_complex(funcname)        \
  template <typename T, typename I>                                                \
  inline auto funcname(const PoolDynamicSparseNumberArray<T, I> & in)   \
  ->typename PoolDynamicSparseNumberArray<T, I>::template rebind<decltype(std::funcname(T())), \
                                                                 I>::other \
  {                                                                     \
    typename PoolDynamicSparseNumberArray<T, I>::template rebind<decltype(std::funcname(T())), \
                                                                 I>::other returnval; \
    *returnval.dsna_ptr = std::funcname(*in.dsna_ptr);                  \
    return returnval;                                                   \
  }

PoolDynamicSparseNumberArray_std_unary_complex(real)
PoolDynamicSparseNumberArray_std_unary_complex(imag)
PoolDynamicSparseNumberArray_std_unary_complex(norm)
} // namespace std

#endif // METAPHYSICL_POOL_DYNAMICSPARSENUMBERARRAY_H
