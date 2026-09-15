#ifndef METAPHYSICL_NUMERIC_LIMITS
#define METAPHYSICL_NUMERIC_LIMITS

#ifdef METAPHYSICL_KOKKOS_COMPILATION

#include "metaphysicl/metaphysicl_device.h"
#include "metaphysicl/ignore_warnings.h"
#include <Kokkos_NumericTraits.hpp>
#include "metaphysicl/restore_warnings.h"

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
