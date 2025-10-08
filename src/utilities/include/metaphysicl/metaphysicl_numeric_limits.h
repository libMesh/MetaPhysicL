#ifndef METAPHYSICL_NUMERIC_LIMITS
#define METAPHYSICL_NUMERIC_LIMITS

#ifdef METAPHYSICL_HAVE_KOKKOS

#include "metaphysicl/metaphysicl_device.h"
#include <Kokkos_NumericTraits.hpp>

namespace MetaPhysicL {
template <typename T> class numeric_limits {
public:
  METAPHYSICL_INLINE static constexpr auto max() {
    return Kokkos::Experimental::finite_max_v<T>;
  }

  METAPHYSICL_INLINE static constexpr auto epsilon() {
    return Kokkos::Experimental::epsilon_v<T>;
  }
};
} // namespace MetaPhysicL

#else

#include <limits>

namespace MetaPhysicL {
template <typename T> class numeric_limits {
public:
  static T min() { return std::numeric_limits<T>::min(); }
  static T max() { return std::numeric_limits<T>::max(); }
  static int digits() { return std::numeric_limits<T>::digits; }
  static int digits10() { return std::numeric_limits<T>::digits10; }
  static bool is_signed() { return std::numeric_limits<T>::is_signed; }
  static bool is_integer() { return std::numeric_limits<T>::is_integer; }
  static bool is_exact() { return std::numeric_limits<T>::is_exact; }
  static int radix() { return std::numeric_limits<T>::radix; }
  static T epsilon() { return std::numeric_limits<T>::epsilon(); }
  static T round_error() { return std::numeric_limits<T>::round_error(); }

  static int min_exponent() { return std::numeric_limits<T>::min_exponent; }
  static int min_exponent10() { return std::numeric_limits<T>::min_exponent10; }
  static int max_exponent() { return std::numeric_limits<T>::max_exponent; }
  static int max_exponent10() { return std::numeric_limits<T>::max_exponent10; }

  static bool has_infinity() { return std::numeric_limits<T>::has_infinity; }
  static bool has_quiet_NaN() { return std::numeric_limits<T>::has_quiet_NaN; }
  static bool has_signaling_NaN() {
    return std::numeric_limits<T>::has_signaling_NaN;
  }
  static std::float_denorm_style has_denorm() {
    return std::numeric_limits<T>::has_denorm;
  }
  static bool has_denorm_loss() {
    return std::numeric_limits<T>::has_denorm_loss;
  }
  static T infinity() { return std::numeric_limits<T>::infinity(); }
  static T quiet_NaN() { return std::numeric_limits<T>::quiet_NaN(); }
  static T signaling_NaN() { return std::numeric_limits<T>::signaling_NaN(); }
  static T denorm_min() { return std::numeric_limits<T>::denorm_min(); }

  static bool is_iec559() { return std::numeric_limits<T>::is_iec559; }
  static bool is_bounded() { return std::numeric_limits<T>::is_bounded; }
  static bool is_modulo() { return std::numeric_limits<T>::is_modulo; }

  static bool traps() { return std::numeric_limits<T>::traps; }
  static bool tinyness_before() {
    return std::numeric_limits<T>::tinyness_before;
  }
  static std::float_round_style round_style() {
    return std::numeric_limits<T>::round_style;
  }
};

} // namespace MetaPhysicL

#endif // METAPHYSICL_HAVE_KOKKOS

#endif // METAPHYSICL_NUMERIC_LIMITS
