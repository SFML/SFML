//
// detail/impl/posix_event.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_POSIX_EVENT_IPP
#define NET_TS_DETAIL_IMPL_POSIX_EVENT_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_PTHREADS)

#include <experimental/__net_ts/detail/posix_event.hpp>
#include <experimental/__net_ts/detail/throw_error.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

posix_event::posix_event()
  : state_(0)
{
#if (defined(__MACH__) && defined(__APPLE__)) \
      || (defined(__ANDROID__) && (__ANDROID_API__ < 21))
  int error = ::pthread_cond_init(&cond_, 0);
#else // (defined(__MACH__) && defined(__APPLE__))
      // || (defined(__ANDROID__) && (__ANDROID_API__ < 21))
  ::pthread_condattr_t attr;
  ::pthread_condattr_init(&attr);
  int error = ::pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
  if (error == 0)
    error = ::pthread_cond_init(&cond_, &attr);
#endif // (defined(__MACH__) && defined(__APPLE__))
       // || (defined(__ANDROID__) && (__ANDROID_API__ < 21))

  std::error_code ec(error,
      std::experimental::net::v1::error::get_system_category());
  std::experimental::net::v1::detail::throw_error(ec, "event");
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_HAS_PTHREADS)

#endif // NET_TS_DETAIL_IMPL_POSIX_EVENT_IPP
