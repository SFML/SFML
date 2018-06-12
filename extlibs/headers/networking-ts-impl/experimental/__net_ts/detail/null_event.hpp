//
// detail/null_event.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_NULL_EVENT_HPP
#define NET_TS_DETAIL_NULL_EVENT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/noncopyable.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class null_event
  : private noncopyable
{
public:
  // Constructor.
  null_event()
  {
  }

  // Destructor.
  ~null_event()
  {
  }

  // Signal the event. (Retained for backward compatibility.)
  template <typename Lock>
  void signal(Lock&)
  {
  }

  // Signal all waiters.
  template <typename Lock>
  void signal_all(Lock&)
  {
  }

  // Unlock the mutex and signal one waiter.
  template <typename Lock>
  void unlock_and_signal_one(Lock&)
  {
  }

  // If there's a waiter, unlock the mutex and signal it.
  template <typename Lock>
  bool maybe_unlock_and_signal_one(Lock&)
  {
    return false;
  }

  // Reset the event.
  template <typename Lock>
  void clear(Lock&)
  {
  }

  // Wait for the event to become signalled.
  template <typename Lock>
  void wait(Lock&)
  {
    do_wait();
  }

  // Timed wait for the event to become signalled.
  template <typename Lock>
  bool wait_for_usec(Lock&, long usec)
  {
    do_wait_for_usec(usec);
    return true;
  }

private:
  NET_TS_DECL static void do_wait();
  NET_TS_DECL static void do_wait_for_usec(long usec);
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/null_event.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_DETAIL_NULL_EVENT_HPP
