#ifndef METAPHYSICL_DEVICE_H
#define METAPHYSICL_DEVICE_H

#ifdef METAPHYSICL_HAVE_KOKKOS

#include <Kokkos_Array.hpp>
#include <Kokkos_Macros.hpp>

#define METAPHYSICL_INLINE KOKKOS_INLINE_FUNCTION
#define METAPHYSICL_IF_ON_HOST(...) KOKKOS_IF_ON_HOST(__VA_ARGS__)
// #define METAPHYSICL_IF_ON_DEVICE(...) KOKKOS_IF_ON_DEVICE(__VA_ARGS__)

namespace MetaPhysicL {
template <typename T> METAPHYSICL_INLINE auto begin(T &t) {
  return Kokkos::begin(t);
}
} // namespace MetaPhysicL

#else

#include <array>

#define METAPHYSICL_INLINE inline
// Helper to remove one level of parentheses
#define METAPHYSICL_UNPAREN(...) __VA_ARGS__

// Make it a standalone statement (no trailing ';' required), and
// unparen so ( ... ; ) becomes ... ;
#define METAPHYSICL_IF_ON_HOST(CODE)                                           \
  if (true) {                                                                  \
    METAPHYSICL_UNPAREN CODE                                                   \
  } else {                                                                     \
  }

// #define METAPHYSICL_IF_ON_DEVICE(device_code) ((void)0)

namespace MetaPhysicL {
template <typename T> auto begin(T &t) { return std::begin(t); }
} // namespace MetaPhysicL
#endif

#endif // METAPHYSICL_DEVICE_H
