#ifndef METAPHYSICL_ALGORITHM_H
#define METAPHYSICL_ALGORITHM_H

#include "metaphysicl/metaphysicl_device.h"

#include <algorithm>
#include <type_traits>

namespace MetaPhysicL::detail {

#ifdef METAPHYSICL_KOKKOS_COMPILATION

template <typename T, typename U>
METAPHYSICL_INLINE constexpr bool less(const T & lhs, const U & rhs)
{
  typedef typename std::remove_const<typename std::remove_reference<T>::type>::type lhs_type;
  typedef typename std::remove_const<typename std::remove_reference<U>::type>::type rhs_type;

  if constexpr (std::is_integral<lhs_type>::value && std::is_integral<rhs_type>::value)
  {
    if constexpr (std::is_signed<lhs_type>::value == std::is_signed<rhs_type>::value)
      return lhs < rhs;
    else if constexpr (std::is_signed<lhs_type>::value)
    {
      if (lhs < 0)
        return true;
      return static_cast<typename std::make_unsigned<lhs_type>::type>(lhs) < rhs;
    }
    else
    {
      if (rhs < 0)
        return false;
      return lhs < static_cast<typename std::make_unsigned<rhs_type>::type>(rhs);
    }
  }
  else
    return lhs < rhs;
}

template <typename ForwardIt, typename T>
METAPHYSICL_INLINE ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T & value)
{
  auto count = last - first;
  while (count > 0)
  {
    const auto step = count / 2;
    auto it = first + step;
    if (less(*it, value))
    {
      first = ++it;
      count -= step + 1;
    }
    else
      count = step;
  }

  return first;
}

template <typename BidirectionalIt1, typename BidirectionalIt2>
METAPHYSICL_INLINE BidirectionalIt2 copy_backward(BidirectionalIt1 first,
                                                  BidirectionalIt1 last,
                                                  BidirectionalIt2 result)
{
  while (first != last)
    *(--result) = *(--last);
  return result;
}

template <typename ForwardIt>
METAPHYSICL_INLINE ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
{
  if (first == last)
    return last;

  auto next = first;
  ++next;
  for (; next != last; ++first, ++next)
    if (*first == *next)
      return first;

  return last;
}

template <typename ForwardIt>
METAPHYSICL_INLINE bool is_sorted(ForwardIt first, ForwardIt last)
{
  if (first != last)
  {
    auto next = first;
    while (++next != last)
    {
      if (less(*next, *first))
        return false;
      first = next;
    }
  }

  return true;
}

#else

template <typename ForwardIt, typename T>
METAPHYSICL_INLINE ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T & value)
{
  return std::lower_bound(first, last, value);
}

template <typename BidirectionalIt1, typename BidirectionalIt2>
METAPHYSICL_INLINE BidirectionalIt2 copy_backward(BidirectionalIt1 first,
                                                  BidirectionalIt1 last,
                                                  BidirectionalIt2 result)
{
  return std::copy_backward(first, last, result);
}

template <typename ForwardIt>
METAPHYSICL_INLINE ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
{
  return std::adjacent_find(first, last);
}

template <typename ForwardIt>
METAPHYSICL_INLINE bool is_sorted(ForwardIt first, ForwardIt last)
{
  return std::is_sorted(first, last);
}

#endif

} // namespace MetaPhysicL::detail

#endif // METAPHYSICL_ALGORITHM_H
