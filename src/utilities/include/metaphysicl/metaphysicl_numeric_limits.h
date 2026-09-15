#ifndef METAPHYSICL_NUMERIC_LIMITS
#define METAPHYSICL_NUMERIC_LIMITS

#ifdef METAPHYSICL_KOKKOS_COMPILATION

#include "metaphysicl/metaphysicl_device.h"
#include "metaphysicl/ignore_warnings.h"
#include <Kokkos_NumericTraits.hpp>
#include "metaphysicl/restore_warnings.h"

#include <limits>
#include <type_traits>

namespace MetaPhysicL {

// Kokkos::Experimental defines its numeric traits only for arithmetic types,
// and several of them only for floating point types.  Where a trait is outside
// those constraints, and for the traits Kokkos has no equivalent of at all
// (the boolean properties, rounding and denormal styles), fall back on
// std::numeric_limits: those members are compile-time constants, so they stay
// usable from device code.
#define METAPHYSICL_KOKKOS_FP_VALUE(NAME)                                      \
  METAPHYSICL_INLINE static constexpr T NAME() {                               \
    if constexpr (std::is_floating_point_v<T>)                                 \
      return Kokkos::Experimental::NAME##_v<T>;                                \
    else                                                                       \
      return std::numeric_limits<T>::NAME();                                   \
  }

#define METAPHYSICL_KOKKOS_ARITHMETIC_TRAIT(NAME, TYPE)                        \
  METAPHYSICL_INLINE static constexpr TYPE NAME() {                            \
    if constexpr (std::is_arithmetic_v<T>)                                     \
      return Kokkos::Experimental::NAME##_v<T>;                                \
    else                                                                       \
      return std::numeric_limits<T>::NAME;                                     \
  }

#define METAPHYSICL_KOKKOS_FP_TRAIT(NAME, TYPE)                                \
  METAPHYSICL_INLINE static constexpr TYPE NAME() {                            \
    if constexpr (std::is_floating_point_v<T>)                                 \
      return Kokkos::Experimental::NAME##_v<T>;                                \
    else                                                                       \
      return std::numeric_limits<T>::NAME;                                     \
  }

#define METAPHYSICL_STD_TRAIT(NAME, TYPE)                                      \
  METAPHYSICL_INLINE static constexpr TYPE NAME() {                            \
    return std::numeric_limits<T>::NAME;                                       \
  }

template <typename T> class numeric_limits {
public:
  // std::numeric_limits<T>::min() is the smallest normalized value for a
  // floating point type and the lowest representable value for an integral
  // one; Kokkos splits those into two traits.
  METAPHYSICL_INLINE static constexpr T min() {
    if constexpr (std::is_floating_point_v<T>)
      return Kokkos::Experimental::norm_min_v<T>;
    else if constexpr (std::is_arithmetic_v<T>)
      return Kokkos::Experimental::finite_min_v<T>;
    else
      return std::numeric_limits<T>::min();
  }

  METAPHYSICL_INLINE static constexpr T max() {
    if constexpr (std::is_arithmetic_v<T>)
      return Kokkos::Experimental::finite_max_v<T>;
    else
      return std::numeric_limits<T>::max();
  }

  METAPHYSICL_KOKKOS_ARITHMETIC_TRAIT(digits, int)
  METAPHYSICL_KOKKOS_ARITHMETIC_TRAIT(digits10, int)
  METAPHYSICL_KOKKOS_ARITHMETIC_TRAIT(radix, int)
  METAPHYSICL_STD_TRAIT(is_signed, bool)
  METAPHYSICL_STD_TRAIT(is_integer, bool)
  METAPHYSICL_STD_TRAIT(is_exact, bool)

  METAPHYSICL_KOKKOS_FP_VALUE(epsilon)
  METAPHYSICL_KOKKOS_FP_VALUE(round_error)

  METAPHYSICL_KOKKOS_FP_TRAIT(min_exponent, int)
  METAPHYSICL_KOKKOS_FP_TRAIT(min_exponent10, int)
  METAPHYSICL_KOKKOS_FP_TRAIT(max_exponent, int)
  METAPHYSICL_KOKKOS_FP_TRAIT(max_exponent10, int)

  METAPHYSICL_STD_TRAIT(has_infinity, bool)
  METAPHYSICL_STD_TRAIT(has_quiet_NaN, bool)
  METAPHYSICL_STD_TRAIT(has_signaling_NaN, bool)
  METAPHYSICL_STD_TRAIT(has_denorm, std::float_denorm_style)
  METAPHYSICL_STD_TRAIT(has_denorm_loss, bool)

  METAPHYSICL_KOKKOS_FP_VALUE(infinity)
  METAPHYSICL_KOKKOS_FP_VALUE(quiet_NaN)
  METAPHYSICL_KOKKOS_FP_VALUE(signaling_NaN)
  METAPHYSICL_KOKKOS_FP_VALUE(denorm_min)

  METAPHYSICL_STD_TRAIT(is_iec559, bool)
  METAPHYSICL_STD_TRAIT(is_bounded, bool)
  METAPHYSICL_STD_TRAIT(is_modulo, bool)

  METAPHYSICL_STD_TRAIT(traps, bool)
  METAPHYSICL_STD_TRAIT(tinyness_before, bool)
  METAPHYSICL_STD_TRAIT(round_style, std::float_round_style)
};

#undef METAPHYSICL_KOKKOS_FP_VALUE
#undef METAPHYSICL_KOKKOS_ARITHMETIC_TRAIT
#undef METAPHYSICL_KOKKOS_FP_TRAIT
#undef METAPHYSICL_STD_TRAIT

} // namespace MetaPhysicL

#else

#include <limits>

namespace MetaPhysicL {
template <typename T> class numeric_limits {
public:
  static T min() { return std::numeric_limits<T>::min(); }
  static T max() { return std::numeric_limits<T>::max(); }
  static constexpr int digits() { return std::numeric_limits<T>::digits; }
  static constexpr int digits10() { return std::numeric_limits<T>::digits10; }
  static constexpr bool is_signed() { return std::numeric_limits<T>::is_signed; }
  static constexpr bool is_integer() { return std::numeric_limits<T>::is_integer; }
  static constexpr bool is_exact() { return std::numeric_limits<T>::is_exact; }
  static constexpr int radix() { return std::numeric_limits<T>::radix; }
  static T epsilon() { return std::numeric_limits<T>::epsilon(); }
  static T round_error() { return std::numeric_limits<T>::round_error(); }

  static constexpr int min_exponent() { return std::numeric_limits<T>::min_exponent; }
  static constexpr int min_exponent10() { return std::numeric_limits<T>::min_exponent10; }
  static constexpr int max_exponent() { return std::numeric_limits<T>::max_exponent; }
  static constexpr int max_exponent10() { return std::numeric_limits<T>::max_exponent10; }

  static constexpr bool has_infinity() { return std::numeric_limits<T>::has_infinity; }
  static constexpr bool has_quiet_NaN() { return std::numeric_limits<T>::has_quiet_NaN; }
  static constexpr bool has_signaling_NaN() {
    return std::numeric_limits<T>::has_signaling_NaN;
  }
  static constexpr std::float_denorm_style has_denorm() {
    return std::numeric_limits<T>::has_denorm;
  }
  static constexpr bool has_denorm_loss() {
    return std::numeric_limits<T>::has_denorm_loss;
  }
  static T infinity() { return std::numeric_limits<T>::infinity(); }
  static T quiet_NaN() { return std::numeric_limits<T>::quiet_NaN(); }
  static T signaling_NaN() { return std::numeric_limits<T>::signaling_NaN(); }
  static T denorm_min() { return std::numeric_limits<T>::denorm_min(); }

  static constexpr bool is_iec559() { return std::numeric_limits<T>::is_iec559; }
  static constexpr bool is_bounded() { return std::numeric_limits<T>::is_bounded; }
  static constexpr bool is_modulo() { return std::numeric_limits<T>::is_modulo; }

  static constexpr bool traps() { return std::numeric_limits<T>::traps; }
  static constexpr bool tinyness_before() {
    return std::numeric_limits<T>::tinyness_before;
  }
  static constexpr std::float_round_style round_style() {
    return std::numeric_limits<T>::round_style;
  }
};

} // namespace MetaPhysicL

#endif // METAPHYSICL_KOKKOS_COMPILATION

#endif // METAPHYSICL_NUMERIC_LIMITS
