#ifndef METAPHYSICL_ALGORITHM_H
#define METAPHYSICL_ALGORITHM_H

#include "metaphysicl/metaphysicl_device.h"

namespace MetaPhysicL::detail {

template <typename ForwardIt, typename T>
METAPHYSICL_INLINE ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T & value)
{
  auto count = last - first;
  while (count > 0)
  {
    const auto step = count / 2;
    auto it = first + step;
    if (*it < value)
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
      if (*next < *first)
        return false;
      first = next;
    }
  }

  return true;
}

} // namespace MetaPhysicL::detail

#endif // METAPHYSICL_ALGORITHM_H
