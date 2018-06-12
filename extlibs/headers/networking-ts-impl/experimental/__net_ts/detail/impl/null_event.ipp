//
// detail/impl/null_event.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_NULL_EVENT_IPP
#define NET_TS_DETAIL_IMPL_NULL_EVENT_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS_RUNTIME)
# include <thread>
#elif defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
# include <experimental/__net_ts/detail/socket_types.hpp>
#else
# include <unistd.h>
# if defined(__hpux)
#  include <sys/time.h>
# endif
# if !defined(__hpux) || defined(__SELECT)
#  include <sys/select.h>
# endif
#endif

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

void null_event::do_wait()
{
#if defined(NET_TS_WINDOWS_RUNTIME)
  std::this_thread::sleep_until((std::chrono::steady_clock::time_point::max)());
#elif defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
  ::Sleep(INFINITE);
#else
  ::pause();
#endif
}

void null_event::do_wait_for_usec(long usec)
{
#if defined(NET_TS_WINDOWS_RUNTIME)
  std::this_thread::sleep_for(std::chrono::microseconds(usec));
#elif defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
  ::Sleep(usec / 1000);
#elif defined(__hpux) && defined(__SELECT)
  timespec ts;
  ts.tv_sec = usec / 1000000;
  ts.tv_nsec = (usec % 1000000) * 1000;
  ::pselect(0, 0, 0, 0, &ts, 0);
#else
  timeval tv;
  tv.tv_sec = usec / 1000000;
  tv.tv_usec = usec % 1000000;
  ::select(0, 0, 0, 0, &tv);
#endif
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_IMPL_NULL_EVENT_IPP
