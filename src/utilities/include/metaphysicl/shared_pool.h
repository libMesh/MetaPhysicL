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

#ifndef METAPHYSICL_SHARED_POOL_H
#define METAPHYSICL_SHARED_POOL_H

// System Includes
#include <stack>
#include <memory>

namespace MetaPhysicL
{

template <class T, typename... Args>
auto
reset(int, T & obj, Args... args) -> decltype(obj.reset(args...), void())
{
  obj.reset(std::forward<Args>(args)...);
}

template <class T, typename... Args>
void
reset(double, T & /*obj*/, Args... /*args*/)
{
}

/**
 *
 * Originally From https://stackoverflow.com/a/27837534/2042320
 *
 * friedmud added variadic templated perfect forwarding to acquire()
 *
 * For an object to be resetable it needs to define a reset() function
 * that takes the same arguments as its constructor.
 */
template <class T>
class SharedPool
{
private:
  struct ExternalDeleter
  {
    explicit ExternalDeleter(std::weak_ptr<SharedPool<T> *> pool) : _pool(pool) {}

    void operator()(T * ptr)
    {
      if (auto _poolptr = _pool.lock())
      {
        try
        {
          (*_poolptr.get())->add(std::unique_ptr<T>{ptr});
          return;
        }
        catch (...)
        {
        }
      }
      std::default_delete<T>{}(ptr);
    }

  private:
    std::weak_ptr<SharedPool<T> *> _pool;
  };

public:
  typedef typename std::unique_ptr<T, ExternalDeleter> PtrType;

  SharedPool() : _this_ptr(new SharedPool<T> *(this)) {}
  virtual ~SharedPool() {}

  void add(std::unique_ptr<T> t) { _pool.push(std::move(t)); }

  template <typename... Args>
  PtrType acquire(Args &&... args)
  {
    // if the pool is empty - create one
    if (_pool.empty())
    {
      _num_created++;
      return std::move(PtrType(new T(std::forward<Args>(args)...),
                               ExternalDeleter{std::weak_ptr<SharedPool<T> *>{_this_ptr}}));
    }
    else
    {
      PtrType tmp(_pool.top().release(),
                  ExternalDeleter{std::weak_ptr<SharedPool<T> *>{_this_ptr}});
      _pool.pop();

      reset(1, *tmp, std::forward<Args>(args)...);

      return tmp;
    }
  }

  bool empty() const { return _pool.empty(); }

  std::size_t size() const { return _pool.size(); }

  std::size_t num_created() const { return _num_created; }

private:
  std::shared_ptr<SharedPool<T> *> _this_ptr;
  std::stack<std::unique_ptr<T>> _pool;

  std::size_t _num_created = 0;
};
}

#endif // METAPHYSICL_SHARED_POOL_H
