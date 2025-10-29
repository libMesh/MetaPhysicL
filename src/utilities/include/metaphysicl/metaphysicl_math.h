// Code created with assistance of ChatGPT-5

// When we can, call C math in the global namespace (device-safe on
// CUDA/HIP). Otherwise make unqualified call to enable ADL (nested
// duals, complexes, etc.). We avoid self-recursion by routing ADL
// through a helper in the nested detail namespace.

#ifndef METAPHYSICL_MATH_H
#define METAPHYSICL_MATH_H

#include "metaphysicl/metaphysicl_device.h"
#ifdef METAPHYSICL_KOKKOS_COMPILATION
#include <Kokkos_Core.hpp>
#else
#include <cmath>
#endif
#include <type_traits>

namespace MetaPhysicL {
namespace math {

#define METAPHYSICL_MATH_DETAIL_UNQUALIFIED_UNARY_STD(NAME)                    \
  namespace detail {                                                           \
  template <class X> METAPHYSICL_INLINE auto unqualified_##NAME(const X &x) {  \
    if constexpr (std::is_arithmetic_v<X>)                                     \
      return std::NAME(x);                                                     \
    else {                                                                     \
      using std::NAME;                                                         \
      return NAME(x);                                                          \
    }                                                                          \
  }                                                                            \
  }

#ifdef METAPHYSICL_KOKKOS_COMPILATION
#define METAPHYSICL_MATH_DETAIL_UNQUALIFIED_UNARY(NAME)                        \
  namespace detail {                                                           \
  template <class X> METAPHYSICL_INLINE auto unqualified_##NAME(const X &x) {  \
    if constexpr (std::is_arithmetic_v<X>)                                     \
      return Kokkos::NAME(x);                                                  \
    else {                                                                     \
      using Kokkos::NAME;                                                      \
      return NAME(x);                                                          \
    }                                                                          \
  }                                                                            \
  }
#else
#define METAPHYSICL_MATH_DETAIL_UNQUALIFIED_UNARY(NAME)                        \
  METAPHYSICL_MATH_DETAIL_UNQUALIFIED_UNARY_STD(NAME)
#endif

#ifdef METAPHYSICL_KOKKOS_COMPILATION
#define METAPHYSICL_MATH_DETAIL_UNQUALIFIED_BINARY(NAME)                       \
  namespace detail {                                                           \
  template <class X, class Y>                                                  \
  METAPHYSICL_INLINE auto unqualified_##NAME(const X &x, const Y &y) {         \
    if constexpr (std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>)          \
      return Kokkos::NAME(x, y);                                               \
    else {                                                                     \
      using Kokkos::NAME;                                                      \
      return NAME(x, y);                                                       \
    }                                                                          \
  }                                                                            \
  }
#else
#define METAPHYSICL_MATH_DETAIL_UNQUALIFIED_BINARY(NAME)                       \
  namespace detail {                                                           \
  template <class X, class Y>                                                  \
  METAPHYSICL_INLINE auto unqualified_##NAME(const X &x, const Y &y) {         \
    if constexpr (std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>)          \
      return std::NAME(x, y);                                                  \
    else {                                                                     \
      using std::NAME;                                                         \
      return NAME(x, y);                                                       \
    }                                                                          \
  }                                                                            \
  }
#endif

#define METAPHYSICL_MATH_UNARY(NAME)                                           \
  METAPHYSICL_MATH_DETAIL_UNQUALIFIED_UNARY(NAME)                              \
  template <class X> METAPHYSICL_INLINE auto NAME(const X &x) {                \
    return detail::unqualified_##NAME(x);                                      \
  }

#define METAPHYSICL_MATH_UNARY_STD(NAME)                                       \
  METAPHYSICL_MATH_DETAIL_UNQUALIFIED_UNARY_STD(NAME)                          \
  template <class X> METAPHYSICL_INLINE auto NAME(const X &x) {                \
    return detail::unqualified_##NAME(x);                                      \
  }

// Macro to call when there is a backing math function in c
#define METAPHYSICL_MATH_BINARY(NAME)                                          \
  METAPHYSICL_MATH_DETAIL_UNQUALIFIED_BINARY(NAME)                             \
  template <class X, class Y>                                                  \
  METAPHYSICL_INLINE auto NAME(const X &x, const Y &y) {                       \
    return detail::unqualified_##NAME(x, y);                                   \
  }

METAPHYSICL_MATH_UNARY(sin)
METAPHYSICL_MATH_UNARY(cos)
METAPHYSICL_MATH_UNARY(tanh)
METAPHYSICL_MATH_UNARY(exp)
METAPHYSICL_MATH_UNARY(log)
METAPHYSICL_MATH_UNARY(log2)
METAPHYSICL_MATH_UNARY(sqrt)
METAPHYSICL_MATH_UNARY(asin)
METAPHYSICL_MATH_UNARY(acos)
METAPHYSICL_MATH_UNARY(atan)
METAPHYSICL_MATH_UNARY(sinh)
METAPHYSICL_MATH_UNARY(cosh)
METAPHYSICL_MATH_UNARY(abs)
METAPHYSICL_MATH_UNARY(isnan)
METAPHYSICL_MATH_UNARY(isinf)
METAPHYSICL_MATH_UNARY(log10)
METAPHYSICL_MATH_UNARY(tan)
METAPHYSICL_MATH_UNARY(ceil)
METAPHYSICL_MATH_UNARY(floor)
METAPHYSICL_MATH_UNARY(fabs)
METAPHYSICL_MATH_UNARY(exp2)
METAPHYSICL_MATH_UNARY(expm1)
METAPHYSICL_MATH_UNARY(log1p)
METAPHYSICL_MATH_UNARY(cbrt)
METAPHYSICL_MATH_UNARY(asinh)
METAPHYSICL_MATH_UNARY(acosh)
METAPHYSICL_MATH_UNARY(atanh)
METAPHYSICL_MATH_UNARY(erf)
METAPHYSICL_MATH_UNARY(erfc)
METAPHYSICL_MATH_UNARY(trunc)
METAPHYSICL_MATH_UNARY(round)
METAPHYSICL_MATH_UNARY(nearbyint)
METAPHYSICL_MATH_UNARY_STD(rint)
METAPHYSICL_MATH_UNARY(real)
METAPHYSICL_MATH_UNARY(imag)
METAPHYSICL_MATH_UNARY_STD(norm)
METAPHYSICL_MATH_UNARY(conj)
// METAPHYSICL_MATH_UNARY_C_NO_STD(fabsf)
// METAPHYSICL_MATH_UNARY_C_NO_STD(fabsl)
// METAPHYSICL_MATH_UNARY_NO_C(expm1f)
// METAPHYSICL_MATH_UNARY_NO_C(expm1l)
// METAPHYSICL_MATH_UNARY_NO_C(sqrtf)
// METAPHYSICL_MATH_UNARY_NO_C(sqrtl)
// METAPHYSICL_MATH_UNARY_NO_C(cbrtf)
// METAPHYSICL_MATH_UNARY_NO_C(cbrtl)
// METAPHYSICL_MATH_UNARY_NO_C(sinf)
// METAPHYSICL_MATH_UNARY_NO_C(sinl)
// METAPHYSICL_MATH_UNARY_NO_C(tanf)
// METAPHYSICL_MATH_UNARY_NO_C(tanl)
// METAPHYSICL_MATH_UNARY_NO_C(asinf)
// METAPHYSICL_MATH_UNARY_NO_C(asinl)
// METAPHYSICL_MATH_UNARY_NO_C(atanf)
// METAPHYSICL_MATH_UNARY_NO_C(atanl)
// METAPHYSICL_MATH_UNARY_NO_C(asinhf)
// METAPHYSICL_MATH_UNARY_NO_C(asinhl)
// METAPHYSICL_MATH_UNARY_NO_C(atanhf)
// METAPHYSICL_MATH_UNARY_NO_C(atanhl)
// METAPHYSICL_MATH_UNARY_NO_C(erff)
// METAPHYSICL_MATH_UNARY_NO_C(erfl)
// METAPHYSICL_MATH_UNARY_NO_C(ceilf)
// METAPHYSICL_MATH_UNARY_NO_C(ceill)
// METAPHYSICL_MATH_UNARY_NO_C(floorf)
// METAPHYSICL_MATH_UNARY_NO_C(floorl)
// METAPHYSICL_MATH_UNARY_NO_C(truncf)
// METAPHYSICL_MATH_UNARY_NO_C(truncl)
// METAPHYSICL_MATH_UNARY_NO_C(roundf)
// METAPHYSICL_MATH_UNARY_NO_C(roundl)
// METAPHYSICL_MATH_UNARY_NO_C(nearbyintf)
// METAPHYSICL_MATH_UNARY_NO_C(nearbyintl)
// METAPHYSICL_MATH_UNARY_NO_C(rintf)
// METAPHYSICL_MATH_UNARY_NO_C(rintl)
// METAPHYSICL_MATH_UNARY_NO_C(expf)
// METAPHYSICL_MATH_UNARY_NO_C(expl)
// METAPHYSICL_MATH_UNARY_NO_C(exp2f)
// METAPHYSICL_MATH_UNARY_NO_C(exp2l)
// METAPHYSICL_MATH_UNARY_NO_C(logf)
// METAPHYSICL_MATH_UNARY_NO_C(logl)
// METAPHYSICL_MATH_UNARY_NO_C(log10f)
// METAPHYSICL_MATH_UNARY_NO_C(log10l)
// METAPHYSICL_MATH_UNARY_NO_C(log2f)
// METAPHYSICL_MATH_UNARY_NO_C(log2l)
// METAPHYSICL_MATH_UNARY_NO_C(log1pf)
// METAPHYSICL_MATH_UNARY_NO_C(log1pl)
// METAPHYSICL_MATH_UNARY_NO_C(cosf)
// METAPHYSICL_MATH_UNARY_NO_C(cosl)
// METAPHYSICL_MATH_UNARY_NO_C(acosf)
// METAPHYSICL_MATH_UNARY_NO_C(acosl)
// METAPHYSICL_MATH_UNARY_NO_C(sinhf)
// METAPHYSICL_MATH_UNARY_NO_C(sinhl)
// METAPHYSICL_MATH_UNARY_NO_C(coshf)
// METAPHYSICL_MATH_UNARY_NO_C(coshl)
// METAPHYSICL_MATH_UNARY_NO_C(tanhf)
// METAPHYSICL_MATH_UNARY_NO_C(tanhl)
// METAPHYSICL_MATH_UNARY_NO_C(acoshf)
// METAPHYSICL_MATH_UNARY_NO_C(acoshl)
// METAPHYSICL_MATH_UNARY_NO_C(erfcf)
// METAPHYSICL_MATH_UNARY_NO_C(erfcl)
METAPHYSICL_MATH_UNARY(tgamma)
METAPHYSICL_MATH_UNARY(lgamma)

METAPHYSICL_MATH_BINARY(pow)
METAPHYSICL_MATH_BINARY(fmod)
METAPHYSICL_MATH_BINARY(atan2)
METAPHYSICL_MATH_BINARY(hypot)
METAPHYSICL_MATH_BINARY(remainder)
METAPHYSICL_MATH_BINARY(fmax)
METAPHYSICL_MATH_BINARY(fmin)
METAPHYSICL_MATH_BINARY(fdim)
METAPHYSICL_MATH_BINARY(max)
METAPHYSICL_MATH_BINARY(min)
// METAPHYSICL_MATH_BINARY_NO_C(fmodf)
// METAPHYSICL_MATH_BINARY_NO_C(fmodl)
// METAPHYSICL_MATH_BINARY_NO_C(remainderf)
// METAPHYSICL_MATH_BINARY_NO_C(remainderl)
// METAPHYSICL_MATH_BINARY_NO_C(fmaxf)
// METAPHYSICL_MATH_BINARY_NO_C(fmaxl)
// METAPHYSICL_MATH_BINARY_NO_C(fminf)
// METAPHYSICL_MATH_BINARY_NO_C(fminl)
// METAPHYSICL_MATH_BINARY_NO_C(fdimf)
// METAPHYSICL_MATH_BINARY_NO_C(fdiml)
// METAPHYSICL_MATH_BINARY_NO_C(hypotf)
// METAPHYSICL_MATH_BINARY_NO_C(hypotl)
// METAPHYSICL_MATH_BINARY_NO_C(atan2f)
// METAPHYSICL_MATH_BINARY_NO_C(atan2l)

} // namespace math
} // namespace MetaPhysicL

#endif // METAPHYSICL_MATH_H
