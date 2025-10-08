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

#ifndef METAPHYSICL_DYNAMIC_ARRAY_WRAPPER_H
#define METAPHYSICL_DYNAMIC_ARRAY_WRAPPER_H

#include "metaphysicl/metaphysicl_asserts.h"
#include "metaphysicl/metaphysicl_config.h"
#include "metaphysicl/metaphysicl_device.h"

#ifdef METAPHYSICL_HAVE_TIMPI
#include "timpi/standard_type_forward.h"
#endif

namespace MetaPhysicL
{
template <template <typename, size_t> class Array, typename T, size_t N>
class DynamicArrayWrapper
{
public:
  typedef std::size_t size_type;

  typedef typename Array<T, N>::value_type value_type;
  typedef typename Array<T, N>::reference reference;
  typedef typename Array<T, N>::const_reference const_reference;

  template <typename U>
  struct rebind
  {
    typedef DynamicArrayWrapper<Array, U, N> type;
  };

  METAPHYSICL_INLINE DynamicArrayWrapper(const DynamicArrayWrapper & src)
  {
    _dynamic_n = src._dynamic_n;
    metaphysicl_assert(_dynamic_n <= N);
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
    for (size_type i = 0; i < _dynamic_n; ++i)
      _data[i] = src[i];
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
  }

  // The Array template we're wrapping typically isn't movable but it's contents might be
  METAPHYSICL_INLINE DynamicArrayWrapper(DynamicArrayWrapper && src)
  {
    _dynamic_n = src._dynamic_n;
    metaphysicl_assert(_dynamic_n <= N);
    for (size_type i = 0; i < _dynamic_n; ++i)
      _data[i] = std::move(src[i]);
  }

  METAPHYSICL_INLINE DynamicArrayWrapper & operator=(const DynamicArrayWrapper & src)
  {
    _dynamic_n = src._dynamic_n;
    metaphysicl_assert(_dynamic_n <= N);
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
    for (size_type i = 0; i < _dynamic_n; ++i)
      _data[i] = src[i];
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
    return *this;
  }

  // A Array isn't movable but it's contents might be
  METAPHYSICL_INLINE DynamicArrayWrapper & operator=(DynamicArrayWrapper && src)
  {
    _dynamic_n = src._dynamic_n;
    metaphysicl_assert(_dynamic_n <= N);
    for (size_type i = 0; i < _dynamic_n; ++i)
      _data[i] = std::move(src[i]);
    return *this;
  }

  DynamicArrayWrapper() = default;

  METAPHYSICL_INLINE auto begin() { return ::MetaPhysicL::begin(_data); }

  METAPHYSICL_INLINE auto begin() const { return ::MetaPhysicL::begin(_data); }

  METAPHYSICL_INLINE auto end()
  {
    metaphysicl_assert(_dynamic_n <= N);
    return ::MetaPhysicL::begin(_data) + _dynamic_n;
  }

  METAPHYSICL_INLINE auto end() const
  {
    metaphysicl_assert(_dynamic_n <= N);
    return ::MetaPhysicL::begin(_data) + _dynamic_n;
  }

  METAPHYSICL_INLINE T & operator[](size_type i)
  {
    metaphysicl_assert(i < N);
    return _data[i];
  }

  METAPHYSICL_INLINE const T & operator[](size_type i) const
  {
    metaphysicl_assert(i < _dynamic_n);
    return _data[i];
  }

  METAPHYSICL_INLINE size_type size() const { return _dynamic_n; }

  METAPHYSICL_INLINE void resize(size_type new_size)
  {
    if (new_size > N)
      metaphysicl_error();
    _dynamic_n = new_size;
  }

  METAPHYSICL_INLINE T * data() { return _data.data(); }

  METAPHYSICL_INLINE const T * data() const { return _data.data(); }

protected:
#ifdef METAPHYSICL_HAVE_TIMPI
  friend class TIMPI::StandardType<DynamicArrayWrapper<T, NType>>;
#endif
  Array<T, N> _data;
  std::size_t _dynamic_n = 0;
};
}

#endif // METAPHYSICL_DYNAMIC_ARRAY_WRAPPER_H
