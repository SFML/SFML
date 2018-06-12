//
// detail/posix_mutex.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_POSIX_MUTEX_HPP
#define NET_TS_DETAIL_POSIX_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_PTHREADS)

#include <pthread.h>
#include <experimental/__net_ts/detail/noncopyable.hpp>
#include <experimental/__net_ts/detail/scoped_lock.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class posix_event;

class posix_mutex
  : private noncopyable
{
public:
  typedef std::experimental::net::v1::detail::scoped_lock<posix_mutex> scoped_lock;

  // Constructor.
  NET_TS_DECL posix_mutex();

  // Destructor.
  ~posix_mutex()
  {
    ::pthread_mutex_destroy(&mutex_); // Ignore EBUSY.
  }

  // Lock the mutex.
  void lock()
  {
    (void)::pthread_mutex_lock(&mutex_); // Ignore EINVAL.
  }

  // Unlock the mutex.
  void unlock()
  {
    (void)::pthread_mutex_unlock(&mutex_); // Ignore EINVAL.
  }

private:
  friend class posix_event;
  ::pthread_mutex_t mutex_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/posix_mutex.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // defined(NET_TS_HAS_PTHREADS)

#endif // NET_TS_DETAIL_POSIX_MUTEX_HPP
