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


#ifndef METAPHYSICL_SHADOWNUMBER_H
#define METAPHYSICL_SHADOWNUMBER_H

#include "metaphysicl/shadownumber_decl.h"

namespace MetaPhysicL {

// Member definitions
template <typename T, typename S>
METAPHYSICL_INLINE
ShadowNumber<T,S>::ShadowNumber() {}

template <typename T, typename S>
template <typename T2, typename S2>
METAPHYSICL_INLINE
ShadowNumber<T,S>::ShadowNumber(const T2& v, const S2& s) : _val(v), _shadow(s) {}

template <typename T, typename S>
template <typename T2>
METAPHYSICL_INLINE
ShadowNumber<T,S>::ShadowNumber(const T2& val) : _val(val), _shadow(val) {}

template <typename T, typename S>
template <typename T2, typename S2>
METAPHYSICL_INLINE
ShadowNumber<T,S>::ShadowNumber(ShadowNumber<T2, S2>& other) : _val(other._val), _shadow(other._shadow) {}

template <typename T, typename S>
METAPHYSICL_INLINE
T& ShadowNumber<T,S>::value() { return _val; }

template <typename T, typename S>
METAPHYSICL_INLINE
const T& ShadowNumber<T,S>::value() const { return _val; }

template <typename T, typename S>
METAPHYSICL_INLINE
S& ShadowNumber<T,S>::shadow() { return _shadow; }

template <typename T, typename S>
METAPHYSICL_INLINE
const S& ShadowNumber<T,S>::shadow() const { return _shadow; }

template <typename T, typename S>
METAPHYSICL_INLINE
bool ShadowNumber<T,S>::boolean_test() const { return _val; }

template <typename T, typename S>
METAPHYSICL_INLINE
ShadowNumber<T,S> ShadowNumber<T,S>::operator- () const { return ShadowNumber<T,S> (-_val, -_shadow); }

template <typename T, typename S>
METAPHYSICL_INLINE
ShadowNumber<T,S> ShadowNumber<T,S>::operator! () const { return ShadowNumber<T,S> (!_val, !_shadow); }


template <typename T, typename S>
template <typename T2, typename S2>
METAPHYSICL_INLINE
ShadowNumber<T,S>& ShadowNumber<T,S>::operator+= (const ShadowNumber<T2,S2>& a)
  { _val += a.value(); _shadow += a.shadow(); return *this; }

template <typename T, typename S>
template <typename T2>
METAPHYSICL_INLINE
ShadowNumber<T,S>& ShadowNumber<T,S>::operator+= (const T2& a)
  { _val += a; _shadow += a; return *this; }

template <typename T, typename S>
template <typename T2, typename S2>
METAPHYSICL_INLINE
ShadowNumber<T,S>& ShadowNumber<T,S>::operator-= (const ShadowNumber<T2,S2>& a)
  { _val -= a.value(); _shadow -= a.shadow(); return *this; }

template <typename T, typename S>
template <typename T2>
METAPHYSICL_INLINE
ShadowNumber<T,S>& ShadowNumber<T,S>::operator-= (const T2& a)
  { _val -= a; _shadow -= a; return *this; }

template <typename T, typename S>
template <typename T2, typename S2>
METAPHYSICL_INLINE
ShadowNumber<T,S>& ShadowNumber<T,S>::operator*= (const ShadowNumber<T2,S2>& a)
  { _val *= a.value(); _shadow *= a.shadow(); return *this; }

template <typename T, typename S>
template <typename T2>
METAPHYSICL_INLINE
ShadowNumber<T,S>& ShadowNumber<T,S>::operator*= (const T2& a)
  { _val *= a; _shadow *= a; return *this; }

template <typename T, typename S>
template <typename T2, typename S2>
METAPHYSICL_INLINE
ShadowNumber<T,S>& ShadowNumber<T,S>::operator/= (const ShadowNumber<T2,S2>& a)
  { _val /= a.value(); _shadow /= a.shadow(); return *this; }

template <typename T, typename S>
template <typename T2>
METAPHYSICL_INLINE
ShadowNumber<T,S>& ShadowNumber<T,S>::operator/= (const T2& a)
  { _val /= a; _shadow /= a; return *this; }


//
// Non-member functions
//

#define ShadowNumber_op(opname) \
template <typename T, typename S, typename T2, typename S2> \
METAPHYSICL_INLINE \
typename CompareTypes<ShadowNumber<T,S>,ShadowNumber<T2,S2> >::supertype \
operator opname (const ShadowNumber<T,S>& a, const ShadowNumber<T2,S2>& b) \
{ \
  typedef typename CompareTypes<ShadowNumber<T,S>,ShadowNumber<T2,S2> >::supertype TS; \
  TS returnval(a); \
  returnval opname##= b; \
  return returnval; \
} \
 \
template <typename T, typename S, typename T2> \
METAPHYSICL_INLINE \
typename CompareTypes<ShadowNumber<T,S>,T2>::supertype \
operator opname (const ShadowNumber<T,S>& a, const T2& b) \
{ \
  typedef typename CompareTypes<ShadowNumber<T,S>,T2>::supertype TS; \
  TS returnval(a); \
  returnval opname##= b; \
  return returnval; \
 \
} \
template <typename T, typename T2, typename S> \
METAPHYSICL_INLINE \
typename CompareTypes<ShadowNumber<T2,S>,T>::supertype \
operator opname (const T& a, const ShadowNumber<T2,S>& b) \
{ \
  typedef typename CompareTypes<ShadowNumber<T2,S>,T>::supertype TS; \
  TS returnval(a); \
  returnval opname##= b; \
  return returnval; \
}

ShadowNumber_op(+)
ShadowNumber_op(-)
ShadowNumber_op(*)
ShadowNumber_op(/)

#define ShadowNumber_operator_binary(opname) \
template <typename T, typename S, typename T2, typename S2> \
METAPHYSICL_INLINE \
ShadowNumber<bool, bool> \
operator opname (const ShadowNumber<T,S>& a, const ShadowNumber<T2,S2>& b) \
{ \
  return ShadowNumber<bool, bool> (a.value() opname b.value(), a.shadow() opname b.shadow()); \
} \
 \
template <typename T, typename S, typename T2> \
METAPHYSICL_INLINE \
typename boostcopy::enable_if_class< \
  typename CompareTypes<ShadowNumber<T,S>,T2>::supertype, \
  ShadowNumber<bool,bool> \
>::type \
operator opname (const ShadowNumber<T,S>& a, const T2& b) \
{ \
  return ShadowNumber<bool, bool> (a.value() opname b, a.shadow() opname b); \
} \
 \
template <typename T, typename T2, typename S> \
METAPHYSICL_INLINE \
typename boostcopy::enable_if_class< \
  typename CompareTypes<ShadowNumber<T2,S>,T>::supertype, \
  ShadowNumber<bool,bool> \
>::type \
operator opname (const T& a, const ShadowNumber<T2,S>& b) \
{ \
  return ShadowNumber<bool, bool> (a opname b.value(), a opname b.shadow()); \
}


ShadowNumber_operator_binary(<)
ShadowNumber_operator_binary(<=)
ShadowNumber_operator_binary(>)
ShadowNumber_operator_binary(>=)
ShadowNumber_operator_binary(==)
ShadowNumber_operator_binary(!=)

template <typename T, typename S>
METAPHYSICL_INLINE
std::ostream&      
operator<< (std::ostream& output, const ShadowNumber<T,S>& a)
{
  return output << '(' << a.value() << ',' << a.shadow() << ')';
}


template <typename T, typename S>
struct RawType<ShadowNumber<T, S> >
{
  typedef typename RawType<T>::value_type value_type;

  static value_type value(const ShadowNumber<T, S>& a) {
    const S max_value = math::max(S(a.value()), a.shadow());
    if (max_value) {
      const S relative_error = (a.value() - a.shadow()) / max_value;
      if (relative_error > 10*numeric_limits<T>::epsilon())
        std::cerr << "Shadow relative error = " << relative_error << std::endl;
    }
    return a.value();
  }
};


// For backwards compatibility we still allow violating the C++
// standard by putting our partial template specializations into
// namespace std.
#ifdef METAPHYSICL_ENABLE_STD_VIOLATION
} // namespace MetaPhysicL

namespace std {

namespace math = MetaPhysicL::math;
using MetaPhysicL::CompareTypes;
using MetaPhysicL::ShadowNumber;
#endif


// Now just combined declaration/definitions

#define ShadowNumber_std_unary(funcname) \
template <typename T, typename S> \
METAPHYSICL_INLINE \
ShadowNumber<T, S> \
funcname (ShadowNumber<T, S> a) \
{ \
  a.value() = math::funcname(a.value()); \
  a.shadow() = math::funcname(a.shadow()); \
  return a; \
}


#define ShadowNumber_std_binary(funcname) \
template <typename T, typename S, typename T2, typename S2> \
METAPHYSICL_INLINE \
typename CompareTypes<ShadowNumber<T,S>,ShadowNumber<T2,S2> >::supertype \
funcname (const ShadowNumber<T,S>& a, const ShadowNumber<T2,S2>& b) \
{ \
  typedef typename CompareTypes<ShadowNumber<T,S>,ShadowNumber<T2,S2> >::supertype TS; \
  return TS (math::funcname(a.value(), b.value()), \
             math::funcname(a.shadow(), b.shadow())); \
} \
 \
template <typename T, typename S> \
METAPHYSICL_INLINE \
ShadowNumber<T,S> \
funcname (const ShadowNumber<T,S>& a, const ShadowNumber<T,S>& b) \
{ \
  return ShadowNumber<T,S> \
    (math::funcname(a.value(), b.value()), \
     math::funcname(a.shadow(), b.shadow())); \
} \
 \
template <typename T, typename S, typename T2> \
METAPHYSICL_INLINE \
typename CompareTypes<ShadowNumber<T,S>,T2>::supertype \
funcname (const ShadowNumber<T,S>& a, const T2& b) \
{ \
  typedef typename CompareTypes<ShadowNumber<T,S>,T2>::supertype TS; \
  return TS (math::funcname(a.value(), b), \
             math::funcname(a.shadow(), b)); \
} \
 \
template <typename T, typename T2, typename S> \
METAPHYSICL_INLINE \
typename CompareTypes<ShadowNumber<T2,S>,T>::supertype \
funcname (const T& a, const ShadowNumber<T2,S>& b) \
{ \
  typedef typename CompareTypes<ShadowNumber<T2,S>,T>::supertype TS; \
  return TS (math::funcname(a, b.value()), \
             math::funcname(a, b.shadow())); \
}


ShadowNumber_std_binary(pow)
ShadowNumber_std_unary(exp)
ShadowNumber_std_unary(log)
ShadowNumber_std_unary(log10)
ShadowNumber_std_unary(sin)
ShadowNumber_std_unary(cos)
ShadowNumber_std_unary(tan)
ShadowNumber_std_unary(asin)
ShadowNumber_std_unary(acos)
ShadowNumber_std_unary(atan)
ShadowNumber_std_binary(atan2)
ShadowNumber_std_unary(sinh)
ShadowNumber_std_unary(cosh)
ShadowNumber_std_unary(tanh)
ShadowNumber_std_unary(sqrt)
ShadowNumber_std_unary(abs)
ShadowNumber_std_unary(fabs)
ShadowNumber_std_binary(max)
ShadowNumber_std_binary(min)
ShadowNumber_std_unary(ceil)
ShadowNumber_std_unary(floor)
ShadowNumber_std_binary(fmod)

#if __cplusplus >= 201103L
ShadowNumber_std_unary(exp2)
ShadowNumber_std_unary(expm1)
ShadowNumber_std_unary(log2)
ShadowNumber_std_unary(log1p)
ShadowNumber_std_unary(cbrt)
ShadowNumber_std_unary(asinh)
ShadowNumber_std_unary(acosh)
ShadowNumber_std_unary(atanh)
ShadowNumber_std_unary(erf)
ShadowNumber_std_unary(erfc)
ShadowNumber_std_unary(tgamma)
ShadowNumber_std_unary(lgamma)
ShadowNumber_std_unary(trunc)
ShadowNumber_std_unary(round)
ShadowNumber_std_unary(nearbyint)
ShadowNumber_std_unary(rint)

ShadowNumber_std_binary(remainder)
ShadowNumber_std_binary(fmax)
ShadowNumber_std_binary(fmin)
ShadowNumber_std_binary(fdim)
ShadowNumber_std_binary(hypot)
#endif // __cplusplus >= 201103L

} // namespace std (deprecated) or MetaPhysicL



#endif // METAPHYSICL_SHADOWNUMBER_H
