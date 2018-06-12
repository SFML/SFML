//
// detail/win_mutex.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_WIN_MUTEX_HPP
#define NET_TS_DETAIL_WIN_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS)

#include <experimental/__net_ts/detail/noncopyable.hpp>
#include <experimental/__net_ts/detail/scoped_lock.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class win_mutex
  : private noncopyable
{
public:
  typedef std::experimental::net::v1::detail::scoped_lock<win_mutex> scoped_lock;

  // Constructor.
  NET_TS_DECL win_mutex();

  // Destructor.
  ~win_mutex()
  {
    ::DeleteCriticalSection(&crit_section_);
  }

  // Lock the mutex.
  void lock()
  {
    ::EnterCriticalSection(&crit_section_);
  }

  // Unlock the mutex.
  void unlock()
  {
    ::LeaveCriticalSection(&crit_section_);
  }

private:
  // Initialisation must be performed in a separate function to the constructor
  // since the compiler does not support the use of structured exceptions and
  // C++ exceptions in the same function.
  NET_TS_DECL int do_init();

  ::CRITICAL_SECTION crit_section_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/win_mutex.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // defined(NET_TS_WINDOWS)

#endif // NET_TS_DETAIL_WIN_MUTEX_HPP
