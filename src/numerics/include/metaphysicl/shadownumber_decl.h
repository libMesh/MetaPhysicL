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
// $Id$
//
//--------------------------------------------------------------------------


#ifndef METAPHYSICL_SHADOWNUMBER_DECL_H
#define METAPHYSICL_SHADOWNUMBER_DECL_H

#include "metaphysicl/metaphysicl_config.h"

#include "metaphysicl/compare_types.h"
#include "metaphysicl/metaphysicl_common.h"
#include "metaphysicl/raw_type.h"
#include "metaphysicl/testable.h"

#include <algorithm>
#include <iostream>
#include <limits>

namespace MetaPhysicL {

template <typename T, typename S>
class ShadowNumber : public safe_bool<ShadowNumber<T,S> >
{
public:
  typedef T value_type;

  typedef S shadow_type;

  ShadowNumber();

  template <typename T2, typename S2>
  ShadowNumber(const T2& v, const S2& s);

  template <typename T2>
  ShadowNumber(const T2& val);

  template <typename T2, typename S2>
  ShadowNumber(ShadowNumber<T2, S2>& other);

  T& value();

  const T& value() const;

  S& shadow();

  const S& shadow() const;

  bool boolean_test() const;

  ShadowNumber<T,S> operator- () const;

  ShadowNumber<T,S> operator! () const;

  template <typename T2, typename S2>
  ShadowNumber<T,S>& operator+= (const ShadowNumber<T2,S2>& a);

  template <typename T2>
  ShadowNumber<T,S>& operator+= (const T2& a);

  template <typename T2, typename S2>
  ShadowNumber<T,S>& operator-= (const ShadowNumber<T2,S2>& a);

  template <typename T2>
  ShadowNumber<T,S>& operator-= (const T2& a);

  template <typename T2, typename S2>
  ShadowNumber<T,S>& operator*= (const ShadowNumber<T2,S2>& a);

  template <typename T2>
  ShadowNumber<T,S>& operator*= (const T2& a);

  template <typename T2, typename S2>
  ShadowNumber<T,S>& operator/= (const ShadowNumber<T2,S2>& a);

  template <typename T2>
  ShadowNumber<T,S>& operator/= (const T2& a);

private:
  T _val;
  S _shadow;
};

//
// Non-member functions
//

#define ShadowNumber_decl_op(opname) \
template <typename T, typename S, typename T2, typename S2> \
inline \
typename CompareTypes<ShadowNumber<T,S>,ShadowNumber<T2,S2> >::supertype \
operator opname (const ShadowNumber<T,S>& a, const ShadowNumber<T2,S2>& b); \
 \
 \
template <typename T, typename S, typename T2> \
inline \
typename CompareTypes<ShadowNumber<T,S>,T2>::supertype \
operator opname (const ShadowNumber<T,S>& a, const T2& b); \
 \
template <typename T, typename T2, typename S> \
inline \
typename CompareTypes<ShadowNumber<T2,S>,T>::supertype \
operator opname (const T& a, const ShadowNumber<T2,S>& b);

ShadowNumber_decl_op(+)
ShadowNumber_decl_op(-)
ShadowNumber_decl_op(*)
ShadowNumber_decl_op(/)

#define ShadowNumber_decl_operator_binary(opname) \
template <typename T, typename S, typename T2, typename S2> \
inline \
ShadowNumber<bool, bool> \
operator opname (const ShadowNumber<T,S>& a, const ShadowNumber<T2,S2>& b); \
 \
template <typename T, typename S, typename T2> \
inline \
typename boostcopy::enable_if_class< \
  typename CompareTypes<ShadowNumber<T,S>,T2>::supertype, \
  ShadowNumber<bool,bool> \
>::type \
operator opname (const ShadowNumber<T,S>& a, const T2& b); \
 \
template <typename T, typename T2, typename S> \
inline \
typename boostcopy::enable_if_class< \
  typename CompareTypes<ShadowNumber<T2,S>,T>::supertype, \
  ShadowNumber<bool,bool> \
>::type \
operator opname (const T& a, const ShadowNumber<T2,S>& b);


ShadowNumber_decl_operator_binary(<)
ShadowNumber_decl_operator_binary(<=)
ShadowNumber_decl_operator_binary(>)
ShadowNumber_decl_operator_binary(>=)
ShadowNumber_decl_operator_binary(==)
ShadowNumber_decl_operator_binary(!=)

template <typename T, typename S>
inline
std::ostream&      
operator<< (std::ostream& output, const ShadowNumber<T,S>& a);


// ScalarTraits, RawType, CompareTypes specializations

template <typename T, typename S>
struct ScalarTraits<ShadowNumber<T, S> >
{
  static const bool value = ScalarTraits<T>::value;
};

#define ShadowNumber_comparisons(templatename) \
template<typename T, typename S, bool reverseorder> \
struct templatename<ShadowNumber<T,S>, ShadowNumber<T,S>, reverseorder> { \
  typedef ShadowNumber<T, S> supertype; \
}; \
 \
template<typename T, typename S, typename T2, typename S2, bool reverseorder> \
struct templatename<ShadowNumber<T,S>, ShadowNumber<T2,S2>, reverseorder> { \
  typedef ShadowNumber<typename Symmetric##templatename<T, T2, reverseorder>::supertype, \
                       typename Symmetric##templatename<S, S2, reverseorder>::supertype> supertype; \
}; \
 \
template<typename T, typename S, typename T2, bool reverseorder> \
struct templatename<ShadowNumber<T, S>, T2, reverseorder, \
                    typename boostcopy::enable_if<BuiltinTraits<T2> >::type> { \
  typedef ShadowNumber<typename Symmetric##templatename<T, T2, reverseorder>::supertype, \
                       typename Symmetric##templatename<S, T2, reverseorder>::supertype> supertype; \
}

ShadowNumber_comparisons(CompareTypes);
ShadowNumber_comparisons(PlusType);
ShadowNumber_comparisons(MinusType);
ShadowNumber_comparisons(MultipliesType);
ShadowNumber_comparisons(DividesType);
ShadowNumber_comparisons(AndType);
ShadowNumber_comparisons(OrType);


template <typename T, typename S>
struct RawType<ShadowNumber<T, S> >;
;


// For backwards compatibility we still allow violating the C++
// standard by putting our partial template specializations into
// namespace std.
#ifdef METAPHYSICL_ENABLE_STD_VIOLATION
} // namespace MetaPhysicL

namespace std {

using MetaPhysicL::CompareTypes;
using MetaPhysicL::ShadowNumber;
#endif


// Now just combined declaration/definitions

#define ShadowNumber_decl_std_unary(funcname) \
template <typename T, typename S> \
inline \
ShadowNumber<T, S> \
funcname (ShadowNumber<T, S> a);


#define ShadowNumber_decl_std_binary(funcname) \
template <typename T, typename S, typename T2, typename S2> \
inline \
typename CompareTypes<ShadowNumber<T,S>,ShadowNumber<T2,S2> >::supertype \
funcname (const ShadowNumber<T,S>& a, const ShadowNumber<T2,S2>& b); \
 \
 \
template <typename T, typename S> \
inline \
ShadowNumber<T,S> \
funcname (const ShadowNumber<T,S>& a, const ShadowNumber<T,S>& b); \
 \
 \
template <typename T, typename S, typename T2> \
inline \
typename CompareTypes<ShadowNumber<T,S>,T2>::supertype \
funcname (const ShadowNumber<T,S>& a, const T2& b); \
 \
 \
template <typename T, typename T2, typename S> \
inline \
typename CompareTypes<ShadowNumber<T2,S>,T>::supertype \
funcname (const T& a, const ShadowNumber<T2,S>& b);


ShadowNumber_decl_std_binary(pow)
ShadowNumber_decl_std_unary(exp)
ShadowNumber_decl_std_unary(log)
ShadowNumber_decl_std_unary(log10)
ShadowNumber_decl_std_unary(sin)
ShadowNumber_decl_std_unary(cos)
ShadowNumber_decl_std_unary(tan)
ShadowNumber_decl_std_unary(asin)
ShadowNumber_decl_std_unary(acos)
ShadowNumber_decl_std_unary(atan)
ShadowNumber_decl_std_binary(atan2)
ShadowNumber_decl_std_unary(sinh)
ShadowNumber_decl_std_unary(cosh)
ShadowNumber_decl_std_unary(tanh)
ShadowNumber_decl_std_unary(sqrt)
ShadowNumber_decl_std_unary(abs)
ShadowNumber_decl_std_unary(fabs)
ShadowNumber_decl_std_binary(max)
ShadowNumber_decl_std_binary(min)
ShadowNumber_decl_std_unary(ceil)
ShadowNumber_decl_std_unary(floor)
ShadowNumber_decl_std_binary(fmod)

#if __cplusplus >= 201103L
ShadowNumber_decl_std_unary(exp2)
ShadowNumber_decl_std_unary(expm1)
ShadowNumber_decl_std_unary(log2)
ShadowNumber_decl_std_unary(log1p)
ShadowNumber_decl_std_unary(cbrt)
ShadowNumber_decl_std_unary(asinh)
ShadowNumber_decl_std_unary(acosh)
ShadowNumber_decl_std_unary(atanh)
ShadowNumber_decl_std_unary(erf)
ShadowNumber_decl_std_unary(erfc)
ShadowNumber_decl_std_unary(tgamma)
ShadowNumber_decl_std_unary(lgamma)
ShadowNumber_decl_std_unary(trunc)
ShadowNumber_decl_std_unary(round)
ShadowNumber_decl_std_unary(nearbyint)
ShadowNumber_decl_std_unary(rint)

ShadowNumber_decl_std_binary(remainder)
ShadowNumber_decl_std_binary(fmax)
ShadowNumber_decl_std_binary(fmin)
ShadowNumber_decl_std_binary(fdim)
ShadowNumber_decl_std_binary(hypot)
#endif // __cplusplus >= 201103L


template <typename T, typename S>
class numeric_limits<ShadowNumber<T, S> > :
  public MetaPhysicL::raw_numeric_limits<ShadowNumber<T, S>, T> {};

} // namespace std (deprecated) or MetaPhysicL



#endif // METAPHYSICL_SHADOWNUMBER_DECL_H
