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

#ifndef METAPHYSICL_POOL_DYNAMICSPARSENUMBERARRAY_DECL_H
#define METAPHYSICL_POOL_DYNAMICSPARSENUMBERARRAY_DECL_H

#include "metaphysicl/dynamicsparsenumberarray_decl.h"
#include "metaphysicl/shared_pool.h"

namespace MetaPhysicL
{
// Forward declarations

// Data type T, index type I
template <typename T, typename I>
class PoolDynamicSparseNumberArray;

// Helper structs

template <typename I1, typename I2, typename S, typename T, bool reverseorder>
struct DotType<PoolDynamicSparseNumberArray<S, I1>,
               PoolDynamicSparseNumberArray<T, I2>,
               reverseorder>
{
  typedef PoolDynamicSparseNumberArray<typename DotType<S, T, reverseorder>::supertype,
                                       typename CompareTypes<I1, I2>::supertype>
      supertype;
};

template <typename I1, typename I2, typename S, typename T, bool reverseorder>
struct OuterProductType<PoolDynamicSparseNumberArray<S, I1>,
                        PoolDynamicSparseNumberArray<T, I2>,
                        reverseorder>
{
  typedef PoolDynamicSparseNumberArray<typename OuterProductType<S, T, reverseorder>::supertype,
                                       typename CompareTypes<I1, I2>::supertype>
      supertype;
};

template <typename T, typename I>
SharedPool<DynamicSparseNumberArray<T, I>> &
getPool();

template <typename S, typename I>
struct SumType<PoolDynamicSparseNumberArray<S, I>>
{
  typedef PoolDynamicSparseNumberArray<typename SumType<S>::supertype, I> supertype;
};

template <typename T, typename I>
class PoolDynamicSparseNumberArray : public safe_bool<PoolDynamicSparseNumberArray<T, I>>
{
public:
  template <typename T2, typename I2 = I>
  struct rebind
  {
    typedef PoolDynamicSparseNumberArray<T2, I2> other;
  };

  PoolDynamicSparseNumberArray();

  PoolDynamicSparseNumberArray(const T & val);

  template <typename T2>
  PoolDynamicSparseNumberArray(const T2 & val);

#if METAPHYSICL_USE_STD_MOVE
  // Move constructors are useful when all your data is on the heap
  PoolDynamicSparseNumberArray(PoolDynamicSparseNumberArray<T, I> && src) = default;

  // Move assignment avoids heap operations too
  PoolDynamicSparseNumberArray & operator=(PoolDynamicSparseNumberArray<T, I> && src) = default;

  // Standard copy operations get implicitly deleted upon move
  // constructor definition, so we manually enable them.
  PoolDynamicSparseNumberArray(const PoolDynamicSparseNumberArray<T, I> & src);

  PoolDynamicSparseNumberArray &
  operator=(const PoolDynamicSparseNumberArray<T, I> & src) = default;

  template <typename T2, typename I2>
  PoolDynamicSparseNumberArray(PoolDynamicSparseNumberArray<T2, I2> && src);
#endif

  template <typename T2, typename I2>
  PoolDynamicSparseNumberArray(const PoolDynamicSparseNumberArray<T2, I2> & src);

  template <typename T2, typename I2>
  PoolDynamicSparseNumberArray<typename DotType<T, T2>::supertype,
                               typename CompareTypes<I, I2>::supertype>
  dot(const PoolDynamicSparseNumberArray<T2, I2> & a) const;

  template <typename T2, typename I2>
  PoolDynamicSparseNumberArray<typename OuterProductType<T, T2>::supertype,
                               typename CompareTypes<I, I2>::supertype>
  outerproduct(const PoolDynamicSparseNumberArray<T2, I2> & a) const;

  typedef T value_type;

  template <unsigned int i>
  struct entry_type
  {
    typedef value_type type;
  };

  typedef I index_value_type;

  typedef std::vector<T> Data;
  typedef std::vector<I> Indices;

  std::size_t size() const;

  void resize(std::size_t s);

  T * raw_data();

  const T * raw_data() const;

  T & raw_at(unsigned int i);

  const T & raw_at(unsigned int i) const;

  I & raw_index(unsigned int i);

  const I & raw_index(unsigned int i) const;

  // FIXME: these encapsulation violations are necessary for std::pow
  // until I can figure out the right friend declaration.
  const Data & nude_data() const;

  Data & nude_data();

  const Indices & nude_indices() const;

  Indices & nude_indices();

  std::size_t runtime_index_query(index_value_type i) const;

  std::size_t runtime_index_of(index_value_type i) const;

  T & operator[](index_value_type i);

  const T & operator[](index_value_type i) const;

  template <unsigned int i>
  typename entry_type<i>::type & get();

  template <unsigned int i>
  const typename entry_type<i>::type & get() const;

  value_type & insert(unsigned int i);

  template <unsigned int i>
  typename entry_type<i>::type & insert();

  template <unsigned int i, typename T2>
  void set(const T2 & val);

  bool boolean_test() const;

  PoolDynamicSparseNumberArray<T, I> operator-() const;

  // Since this is a dynamically allocated sparsity pattern, we can
  // increase it as needed to support e.g. operator+=
  template <typename Indices2>
  void sparsity_union(const Indices2 & new_indices);

  // Since this is a dynamically allocated sparsity pattern, we can
  // decrease it when possible for efficiency
  template <typename Indices2>
  void sparsity_intersection(const Indices2 & new_indices);

  // Since this is a dynamically allocated sparsity pattern, we can
  // decrease it when possible for efficiency. This method will remove
  // any index-data pairs for whom the data entry is less than or equal
  // to the prescribed tolerance
  void sparsity_trim(value_type tolerance = 0);

  void clear();

  // Not defineable since !0 != 0
  // SubType<SubTypeArgs...> operator! () const;

  template <typename T2, typename I2>
  PoolDynamicSparseNumberArray<T, I> & operator+=(const PoolDynamicSparseNumberArray<T2, I2> & a);

  template <typename T2, typename I2>
  PoolDynamicSparseNumberArray<T, I> & operator-=(const PoolDynamicSparseNumberArray<T2, I2> & a);

  template <typename T2, typename I2>
  PoolDynamicSparseNumberArray<T, I> & operator*=(const PoolDynamicSparseNumberArray<T2, I2> & a);

  template <typename T2, typename I2>
  PoolDynamicSparseNumberArray<T, I> & operator/=(const PoolDynamicSparseNumberArray<T2, I2> & a);

  template <typename T2>
  PoolDynamicSparseNumberArray<T, I> & operator*=(const T2 & a);

  template <typename T2>
  PoolDynamicSparseNumberArray<T, I> & operator/=(const T2 & a);

  typename SharedPool<DynamicSparseNumberArray<T, I>>::PtrType dsna_ptr;
};

//
// Non-member functions
//

template <typename BoolT, typename BoolI, typename T, typename I, typename T2, typename I2>
inline typename PoolDynamicSparseNumberArray<T, I>::template rebind<
    typename CompareTypes<T, T2>::supertype,
    typename CompareTypes<I, I2>::supertype>::other
if_else(const PoolDynamicSparseNumberArray<BoolT, BoolI> & condition,
        const PoolDynamicSparseNumberArray<T, I> & if_true,
        const PoolDynamicSparseNumberArray<T2, I2> & if_false);

#define PoolDynamicSparseNumberArray_decl_op_ab(opname, functorname)                               \
  template <typename T, typename T2, typename I, typename I2>                                      \
  inline typename Symmetric##functorname##Type<PoolDynamicSparseNumberArray<T, I>,                 \
                                               PoolDynamicSparseNumberArray<T2, I2>>::supertype    \
  operator opname(const PoolDynamicSparseNumberArray<T, I> & a,                                    \
                  const PoolDynamicSparseNumberArray<T2, I2> & b)

#if METAPHYSICL_USE_STD_MOVE

#define PoolDynamicSparseNumberArray_decl_op(opname, functorname)                                  \
  PoolDynamicSparseNumberArray_decl_op_ab(opname, functorname);                                    \
                                                                                                   \
  template <typename T, typename I, typename T2, typename I2>                                      \
  inline typename Symmetric##functorname##Type<PoolDynamicSparseNumberArray<T, I>,                 \
                                               PoolDynamicSparseNumberArray<T2, I2>>::supertype    \
  operator opname(PoolDynamicSparseNumberArray<T, I> && a,                                         \
                  const PoolDynamicSparseNumberArray<T2, I2> & b)

#else

#define PoolDynamicSparseNumberArray_decl_op(opname, functorname)                                  \
  PoolDynamicSparseNumberArray_decl_op_ab(opname, functorname)

#endif

// Let's also allow scalar times vector.
// Scalar plus vector, etc. remain undefined in the sparse context.

template <typename T, typename I, typename T2>
inline typename MultipliesType<PoolDynamicSparseNumberArray<T, I>, T2, true>::supertype
operator*(const T2 & a, const PoolDynamicSparseNumberArray<T, I> & b);

template <typename T, typename I, typename T2>
inline typename MultipliesType<PoolDynamicSparseNumberArray<T, I>, T2>::supertype
operator*(const PoolDynamicSparseNumberArray<T, I> & a, const T2 & b);

template <typename T, typename I, typename T2>
inline typename DividesType<PoolDynamicSparseNumberArray<T, I>, T2>::supertype
operator/(const PoolDynamicSparseNumberArray<T, I> & a, const T2 & b);

#if METAPHYSICL_USE_STD_MOVE

template <typename T, typename I, typename T2>
inline typename MultipliesType<PoolDynamicSparseNumberArray<T, I>, T2>::supertype
operator*(PoolDynamicSparseNumberArray<T, I> && a, const T2 & b);

template <typename T, typename I, typename T2>
inline typename DividesType<PoolDynamicSparseNumberArray<T, I>, T2>::supertype
operator/(PoolDynamicSparseNumberArray<T, I> && a, const T2 & b);

#endif

#define PoolDynamicSparseNumberArray_decl_operator_binary(opname, functorname)                     \
  template <typename T, typename I, typename T2, typename I2>                                      \
  inline typename PoolDynamicSparseNumberArray<T, I>::                                             \
      template rebind<bool, typename CompareTypes<I, I2>::supertype>::other                        \
      operator opname(const PoolDynamicSparseNumberArray<T, I> & a,                                \
                      const PoolDynamicSparseNumberArray<T2, I2> & b);                             \
                                                                                                   \
  template <typename T, typename I, typename T2>                                                   \
  inline typename PoolDynamicSparseNumberArray<T, I>::template rebind<bool>::other                 \
  operator opname(const PoolDynamicSparseNumberArray<T, I> & a, const T2 & b);                     \
                                                                                                   \
  template <typename T, typename I, typename T2>                                                   \
  inline typename PoolDynamicSparseNumberArray<T, I>::template rebind<bool>::other                 \
  operator opname(const T2 & a, const PoolDynamicSparseNumberArray<T, I> & b);

// NOTE: unary functions for which 0-op-0 is true are undefined compile-time
// errors, because there's no efficient way to have them make sense in
// the sparse context.

PoolDynamicSparseNumberArray_decl_operator_binary(<, less);
// PoolDynamicSparseNumberArray_decl_operator_binary(<=);
PoolDynamicSparseNumberArray_decl_operator_binary(>, greater);
// PoolDynamicSparseNumberArray_decl_operator_binary(>=);
// PoolDynamicSparseNumberArray_decl_operator_binary(==);
PoolDynamicSparseNumberArray_decl_operator_binary(!=, not_equal_to);

// FIXME - make && an intersection rather than a union for efficiency
PoolDynamicSparseNumberArray_decl_operator_binary(&&, logical_and);
PoolDynamicSparseNumberArray_decl_operator_binary(||, logical_or);

template <typename T, typename I>
inline std::ostream & operator<<(std::ostream & output,
                                 const PoolDynamicSparseNumberArray<T, I> & a);

// CompareTypes, RawType, ValueType specializations

#define PoolDynamicSparseNumberArray_comparisons(templatename)                                     \
  template <typename T, typename I, bool reverseorder>                                             \
  struct templatename<PoolDynamicSparseNumberArray<T, I>,                                          \
                      PoolDynamicSparseNumberArray<T, I>,                                          \
                      reverseorder>                                                                \
  {                                                                                                \
    typedef PoolDynamicSparseNumberArray<T, I> supertype;                                          \
  };                                                                                               \
                                                                                                   \
  template <typename T, typename I, typename T2, typename I2, bool reverseorder>                   \
  struct templatename<PoolDynamicSparseNumberArray<T, I>,                                          \
                      PoolDynamicSparseNumberArray<T2, I2>,                                        \
                      reverseorder>                                                                \
  {                                                                                                \
    typedef typename PoolDynamicSparseNumberArray<T, I>::template rebind<                          \
        typename Symmetric##templatename<T, T2, reverseorder>::supertype,                          \
        typename CompareTypes<I, I2>::supertype>::other supertype;                                 \
  };                                                                                               \
                                                                                                   \
  template <typename T2, typename T, typename I, bool reverseorder>                                \
  struct templatename<PoolDynamicSparseNumberArray<T, I>,                                          \
                      T2,                                                                          \
                      reverseorder,                                                                \
                      typename boostcopy::enable_if<BuiltinTraits<T2>>::type>                      \
  {                                                                                                \
    typedef typename PoolDynamicSparseNumberArray<T, I>::template rebind<                          \
        typename Symmetric##templatename<T, T2, reverseorder>::supertype,                          \
        I>::other supertype;                                                                       \
  }

} // namespace MetaPhysicL

namespace std
{

using MetaPhysicL::CompareTypes;
using MetaPhysicL::PoolDynamicSparseNumberArray;
using MetaPhysicL::SymmetricCompareTypes;

#define PoolDynamicSparseNumberArray_decl_std_unary(funcname)                                      \
  template <typename T, typename I>                                                                \
  inline PoolDynamicSparseNumberArray<T, I> funcname(                                              \
      const PoolDynamicSparseNumberArray<T, I> & a);

#define PoolDynamicSparseNumberArray_decl_std_binary_union(funcname)                               \
  template <typename T, typename I, typename T2, typename I2>                                      \
  inline typename PoolDynamicSparseNumberArray<T, I>::template rebind<                             \
      typename SymmetricCompareTypes<T, T2>::supertype,                                            \
      typename CompareTypes<I, I2>::supertype>::other                                              \
  funcname(const PoolDynamicSparseNumberArray<T, I> & a,                                           \
           const PoolDynamicSparseNumberArray<T2, I2> & b);                                        \
                                                                                                   \
  template <typename T, typename I, typename T2>                                                   \
  inline typename PoolDynamicSparseNumberArray<T, I>::                                             \
      template rebind<typename SymmetricCompareTypes<T, T2>::supertype, I>::other                  \
      funcname(const PoolDynamicSparseNumberArray<T, I> & a, const T2 & b);                        \
                                                                                                   \
  template <typename T, typename I, typename T2>                                                   \
  inline typename PoolDynamicSparseNumberArray<T, I>::                                             \
      template rebind<typename SymmetricCompareTypes<T2, T>::supertype, I>::other                  \
      funcname(const T2 & a, const PoolDynamicSparseNumberArray<T, I> & b)

#define PoolDynamicSparseNumberArray_decl_fl_unary(funcname)                                       \
  PoolDynamicSparseNumberArray_decl_std_unary(funcname##f);                                        \
  PoolDynamicSparseNumberArray_decl_std_unary(funcname##l)

#define PoolDynamicSparseNumberArray_decl_stdfl_unary(funcname)                                    \
  PoolDynamicSparseNumberArray_decl_std_unary(funcname);                                           \
  PoolDynamicSparseNumberArray_decl_fl_unary(funcname)

#define PoolDynamicSparseNumberArray_decl_fl_binary_union(funcname)                                \
  PoolDynamicSparseNumberArray_decl_std_binary_union(funcname##f);                                 \
  PoolDynamicSparseNumberArray_decl_std_binary_union(funcname##l)

#define PoolDynamicSparseNumberArray_decl_stdfl_binary_union(funcname)                             \
  PoolDynamicSparseNumberArray_decl_std_binary_union(funcname);                                    \
  PoolDynamicSparseNumberArray_decl_fl_binary_union(funcname)

// Pow needs its own specialization, both to avoid being confused by
// pow<T1,T2> and because pow(x,0) isn't 0.
template <typename T, typename I, typename T2>
inline typename PoolDynamicSparseNumberArray<T, I>::
    template rebind<typename SymmetricCompareTypes<T, T2>::supertype, I>::other
    pow(const PoolDynamicSparseNumberArray<T, I> & a, const T2 & b);

// NOTE: unary functions for which f(0) != 0 are undefined compile-time
// errors, because there's no efficient way to have them make sense in
// the sparse context.

// PoolDynamicSparseNumberArray_decl_std_binary(pow); // separate definition
// PoolDynamicSparseNumberArray_decl_std_unary(exp);
// PoolDynamicSparseNumberArray_decl_std_unary(log);
// PoolDynamicSparseNumberArray_decl_std_unary(log10);
PoolDynamicSparseNumberArray_decl_std_unary(sin);
// PoolDynamicSparseNumberArray_decl_std_unary(cos);
PoolDynamicSparseNumberArray_decl_std_unary(tan);
PoolDynamicSparseNumberArray_decl_std_unary(asin);
// PoolDynamicSparseNumberArray_decl_std_unary(acos);
PoolDynamicSparseNumberArray_decl_std_unary(atan);
PoolDynamicSparseNumberArray_decl_std_binary_union(atan2);
PoolDynamicSparseNumberArray_decl_std_unary(sinh);
// PoolDynamicSparseNumberArray_decl_std_unary(cosh);
PoolDynamicSparseNumberArray_decl_std_unary(tanh);
PoolDynamicSparseNumberArray_decl_std_unary(sqrt);
PoolDynamicSparseNumberArray_decl_std_unary(abs);
PoolDynamicSparseNumberArray_decl_std_unary(fabs);
PoolDynamicSparseNumberArray_decl_std_binary_union(max);
PoolDynamicSparseNumberArray_decl_std_binary_union(min);
PoolDynamicSparseNumberArray_decl_std_unary(ceil);
PoolDynamicSparseNumberArray_decl_std_unary(floor);
PoolDynamicSparseNumberArray_decl_std_binary_union(fmod); // TODO: optimize this

#if __cplusplus >= 201103L
PoolDynamicSparseNumberArray_decl_std_unary(llabs);
PoolDynamicSparseNumberArray_decl_std_unary(imaxabs);
PoolDynamicSparseNumberArray_decl_fl_unary(fabs);
// PoolDynamicSparseNumberArray_decl_fl_unary(exp);
// PoolDynamicSparseNumberArray_decl_stdfl_unary(exp2);
PoolDynamicSparseNumberArray_decl_stdfl_unary(expm1);
// PoolDynamicSparseNumberArray_decl_fl_unary(log);
// PoolDynamicSparseNumberArray_decl_fl_unary(log10);
// PoolDynamicSparseNumberArray_decl_stdfl_unary(log2);
// PoolDynamicSparseNumberArray_decl_stdfl_unary(log1p);
PoolDynamicSparseNumberArray_decl_fl_unary(sqrt);
PoolDynamicSparseNumberArray_decl_stdfl_unary(cbrt);
PoolDynamicSparseNumberArray_decl_fl_unary(sin);
// PoolDynamicSparseNumberArray_decl_fl_unary(cos);
PoolDynamicSparseNumberArray_decl_fl_unary(tan);
PoolDynamicSparseNumberArray_decl_fl_unary(asin);
// PoolDynamicSparseNumberArray_decl_fl_unary(acos);
PoolDynamicSparseNumberArray_decl_fl_unary(atan);
PoolDynamicSparseNumberArray_decl_fl_unary(sinh);
// PoolDynamicSparseNumberArray_decl_fl_unary(cosh);
PoolDynamicSparseNumberArray_decl_fl_unary(tanh);
PoolDynamicSparseNumberArray_decl_stdfl_unary(asinh);
// PoolDynamicSparseNumberArray_decl_stdfl_unary(acosh);
PoolDynamicSparseNumberArray_decl_stdfl_unary(atanh);
PoolDynamicSparseNumberArray_decl_stdfl_unary(erf);
// PoolDynamicSparseNumberArray_decl_stdfl_unary(erfc);
PoolDynamicSparseNumberArray_decl_fl_unary(ceil);
PoolDynamicSparseNumberArray_decl_fl_unary(floor);
PoolDynamicSparseNumberArray_decl_stdfl_unary(trunc);
PoolDynamicSparseNumberArray_decl_stdfl_unary(round);
PoolDynamicSparseNumberArray_decl_stdfl_unary(nearbyint);
PoolDynamicSparseNumberArray_decl_stdfl_unary(rint);

PoolDynamicSparseNumberArray_decl_fl_binary_union(fmod);
PoolDynamicSparseNumberArray_decl_std_binary_union(remainder); // TODO: optimize this
PoolDynamicSparseNumberArray_decl_stdfl_binary_union(fmax);
PoolDynamicSparseNumberArray_decl_stdfl_binary_union(fmin);
PoolDynamicSparseNumberArray_decl_stdfl_binary_union(fdim);
PoolDynamicSparseNumberArray_decl_stdfl_binary_union(hypot);
PoolDynamicSparseNumberArray_decl_fl_binary_union(atan2);
#endif // __cplusplus >= 201103L

#define PoolDynamicSparseNumberArray_decl_std_unary_complex(funcname)                              \
  template <typename T, typename I>                                                                \
  inline auto funcname(const PoolDynamicSparseNumberArray<T, I> & in)                              \
      ->typename PoolDynamicSparseNumberArray<T, I>::template rebind<decltype(std::funcname(T())), \
                                                                     I>::other

PoolDynamicSparseNumberArray_decl_std_unary_complex(real);
PoolDynamicSparseNumberArray_decl_std_unary_complex(imag);
PoolDynamicSparseNumberArray_decl_std_unary_complex(norm);
} // namespace std

// Analogs of dynamicsparsenumberarray_decl.h non-member functions

namespace MetaPhysicL
{
// template <typename T, typename I, typename I2>
// inline
// DynamicSparseNumberArray<DynamicSparseNumberArray<T, I>, I2>
// transpose(const DynamicSparseNumberArray<DynamicSparseNumberArray<T, I2>, I>& /*a*/);


// template <typename T, typename I>
// DynamicSparseNumberArray<typename SumType<T>::supertype, I>
// sum (const DynamicSparseNumberArray<T, I> &a);



// DynamicSparseNumberBase_decl_op(DynamicSparseNumberArray, +, Plus)       // Union)
// DynamicSparseNumberBase_decl_op(DynamicSparseNumberArray, -, Minus)      // Union)
// DynamicSparseNumberBase_decl_op(DynamicSparseNumberArray, *, Multiplies) // Intersection)
// DynamicSparseNumberBase_decl_op(DynamicSparseNumberArray, /, Divides)    // First)


// // CompareTypes, RawType, ValueType specializations

PoolDynamicSparseNumberArray_comparisons(CompareTypes);
PoolDynamicSparseNumberArray_comparisons(PlusType);
PoolDynamicSparseNumberArray_comparisons(MinusType);
PoolDynamicSparseNumberArray_comparisons(MultipliesType);
PoolDynamicSparseNumberArray_comparisons(DividesType);
PoolDynamicSparseNumberArray_comparisons(AndType);
PoolDynamicSparseNumberArray_comparisons(OrType);


template <typename T, typename I>
struct RawType<PoolDynamicSparseNumberArray<T, I> >
{
  typedef PoolDynamicSparseNumberArray<typename RawType<T>::value_type, I> value_type;

  static value_type value(const PoolDynamicSparseNumberArray<T, I>& a);
};

template <typename T, typename I>
struct ValueType<PoolDynamicSparseNumberArray<T, I> >
{
  typedef typename ValueType<T>::type type;
};

template <typename T, typename I, typename U>
struct ReplaceAlgebraicType<PoolDynamicSparseNumberArray<T, I>, U>
{
  typedef PoolDynamicSparseNumberArray<typename ReplaceAlgebraicType<T, U>::type, I> type;
};

} // namespace MetaPhysicL


namespace std {

using MetaPhysicL::PoolDynamicSparseNumberArray;

template <typename T, typename I>
class numeric_limits<PoolDynamicSparseNumberArray<T, I> > :
  public MetaPhysicL::raw_numeric_limits<PoolDynamicSparseNumberArray<T, I>, T> {};
} // namespace std

#endif // METAPHYSICL_POOL_DYNAMICSPARSENUMBERARRAY_DECL_H
