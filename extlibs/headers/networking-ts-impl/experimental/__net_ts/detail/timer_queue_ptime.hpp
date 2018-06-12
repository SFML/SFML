//
// detail/timer_queue_ptime.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_TIMER_QUEUE_PTIME_HPP
#define NET_TS_DETAIL_TIMER_QUEUE_PTIME_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_BOOST_DATE_TIME)

#include <experimental/__net_ts/time_traits.hpp>
#include <experimental/__net_ts/detail/timer_queue.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

struct forwarding_posix_time_traits : time_traits<boost::posix_time::ptime> {};

// Template specialisation for the commonly used instantation.
template <>
class timer_queue<time_traits<boost::posix_time::ptime> >
  : public timer_queue_base
{
public:
  // The time type.
  typedef boost::posix_time::ptime time_type;

  // The duration type.
  typedef boost::posix_time::time_duration duration_type;

  // Per-timer data.
  typedef timer_queue<forwarding_posix_time_traits>::per_timer_data
    per_timer_data;

  // Constructor.
  NET_TS_DECL timer_queue();

  // Destructor.
  NET_TS_DECL virtual ~timer_queue();

  // Add a new timer to the queue. Returns true if this is the timer that is
  // earliest in the queue, in which case the reactor's event demultiplexing
  // function call may need to be interrupted and restarted.
  NET_TS_DECL bool enqueue_timer(const time_type& time,
      per_timer_data& timer, wait_op* op);

  // Whether there are no timers in the queue.
  NET_TS_DECL virtual bool empty() const;

  // Get the time for the timer that is earliest in the queue.
  NET_TS_DECL virtual long wait_duration_msec(long max_duration) const;

  // Get the time for the timer that is earliest in the queue.
  NET_TS_DECL virtual long wait_duration_usec(long max_duration) const;

  // Dequeue all timers not later than the current time.
  NET_TS_DECL virtual void get_ready_timers(op_queue<operation>& ops);

  // Dequeue all timers.
  NET_TS_DECL virtual void get_all_timers(op_queue<operation>& ops);

  // Cancel and dequeue operations for the given timer.
  NET_TS_DECL std::size_t cancel_timer(
      per_timer_data& timer, op_queue<operation>& ops,
      std::size_t max_cancelled = (std::numeric_limits<std::size_t>::max)());

  // Move operations from one timer to another, empty timer.
  NET_TS_DECL void move_timer(per_timer_data& target,
      per_timer_data& source);

private:
  timer_queue<forwarding_posix_time_traits> impl_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/timer_queue_ptime.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // defined(NET_TS_HAS_BOOST_DATE_TIME)

#endif // NET_TS_DETAIL_TIMER_QUEUE_PTIME_HPP
