//
// detail/win_event.ipp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_WIN_EVENT_IPP
#define NET_TS_DETAIL_IMPL_WIN_EVENT_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS)

#include <experimental/__net_ts/detail/throw_error.hpp>
#include <experimental/__net_ts/detail/win_event.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

win_event::win_event()
  : state_(0)
{
#if defined(NET_TS_WINDOWS_APP)
  events_[0] = ::CreateEventExW(0, 0,
      CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
#else // defined(NET_TS_WINDOWS_APP)
  events_[0] = ::CreateEventW(0, true, false, 0);
#endif // defined(NET_TS_WINDOWS_APP)
  if (!events_[0])
  {
    DWORD last_error = ::GetLastError();
    std::error_code ec(last_error,
        std::experimental::net::v1::error::get_system_category());
    std::experimental::net::v1::detail::throw_error(ec, "event");
  }

#if defined(NET_TS_WINDOWS_APP)
  events_[1] = ::CreateEventExW(0, 0, 0, EVENT_ALL_ACCESS);
#else // defined(NET_TS_WINDOWS_APP)
  events_[1] = ::CreateEventW(0, false, false, 0);
#endif // defined(NET_TS_WINDOWS_APP)
  if (!events_[1])
  {
    DWORD last_error = ::GetLastError();
    ::CloseHandle(events_[0]);
    std::error_code ec(last_error,
        std::experimental::net::v1::error::get_system_category());
    std::experimental::net::v1::detail::throw_error(ec, "event");
  }
}

win_event::~win_event()
{
  ::CloseHandle(events_[0]);
  ::CloseHandle(events_[1]);
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_WINDOWS)

#endif // NET_TS_DETAIL_IMPL_WIN_EVENT_IPP
