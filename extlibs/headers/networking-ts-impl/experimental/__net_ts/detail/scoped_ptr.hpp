//
// detail/scoped_ptr.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_SCOPED_PTR_HPP
#define NET_TS_DETAIL_SCOPED_PTR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename T>
class scoped_ptr
{
public:
  // Constructor.
  explicit scoped_ptr(T* p = 0)
    : p_(p)
  {
  }

  // Destructor.
  ~scoped_ptr()
  {
    delete p_;
  }

  // Access.
  T* get()
  {
    return p_;
  }

  // Access.
  T* operator->()
  {
    return p_;
  }

  // Dereference.
  T& operator*()
  {
    return *p_;
  }

  // Reset pointer.
  void reset(T* p = 0)
  {
    delete p_;
    p_ = p;
  }

  // Release ownership of the pointer.
  T* release()
  {
    T* tmp = p_;
    p_ = 0;
    return tmp;
  }

private:
  // Disallow copying and assignment.
  scoped_ptr(const scoped_ptr&);
  scoped_ptr& operator=(const scoped_ptr&);

  T* p_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_SCOPED_PTR_HPP
