//
// detail/timer_queue_set.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_TIMER_QUEUE_SET_HPP
#define NET_TS_DETAIL_TIMER_QUEUE_SET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/timer_queue_base.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class timer_queue_set
{
public:
  // Constructor.
  NET_TS_DECL timer_queue_set();

  // Add a timer queue to the set.
  NET_TS_DECL void insert(timer_queue_base* q);

  // Remove a timer queue from the set.
  NET_TS_DECL void erase(timer_queue_base* q);

  // Determine whether all queues are empty.
  NET_TS_DECL bool all_empty() const;

  // Get the wait duration in milliseconds.
  NET_TS_DECL long wait_duration_msec(long max_duration) const;

  // Get the wait duration in microseconds.
  NET_TS_DECL long wait_duration_usec(long max_duration) const;

  // Dequeue all ready timers.
  NET_TS_DECL void get_ready_timers(op_queue<operation>& ops);

  // Dequeue all timers.
  NET_TS_DECL void get_all_timers(op_queue<operation>& ops);

private:
  timer_queue_base* first_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/timer_queue_set.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_DETAIL_TIMER_QUEUE_SET_HPP
