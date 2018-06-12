//
// detail/deadline_timer_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_DEADLINE_TIMER_SERVICE_HPP
#define NET_TS_DETAIL_DEADLINE_TIMER_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <cstddef>
#include <experimental/__net_ts/error.hpp>
#include <experimental/__net_ts/io_context.hpp>
#include <experimental/__net_ts/detail/bind_handler.hpp>
#include <experimental/__net_ts/detail/fenced_block.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/detail/noncopyable.hpp>
#include <experimental/__net_ts/detail/socket_ops.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>
#include <experimental/__net_ts/detail/timer_queue.hpp>
#include <experimental/__net_ts/detail/timer_queue_ptime.hpp>
#include <experimental/__net_ts/detail/timer_scheduler.hpp>
#include <experimental/__net_ts/detail/wait_handler.hpp>
#include <experimental/__net_ts/detail/wait_op.hpp>

#if defined(NET_TS_WINDOWS_RUNTIME)
# include <chrono>
# include <thread>
#endif // defined(NET_TS_WINDOWS_RUNTIME)

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename Time_Traits>
class deadline_timer_service
  : public service_base<deadline_timer_service<Time_Traits> >
{
public:
  // The time type.
  typedef typename Time_Traits::time_type time_type;

  // The duration type.
  typedef typename Time_Traits::duration_type duration_type;

  // The implementation type of the timer. This type is dependent on the
  // underlying implementation of the timer service.
  struct implementation_type
    : private std::experimental::net::v1::detail::noncopyable
  {
    time_type expiry;
    bool might_have_pending_waits;
    typename timer_queue<Time_Traits>::per_timer_data timer_data;
  };

  // Constructor.
  deadline_timer_service(std::experimental::net::v1::io_context& io_context)
    : service_base<deadline_timer_service<Time_Traits> >(io_context),
      scheduler_(std::experimental::net::v1::use_service<timer_scheduler>(io_context))
  {
    scheduler_.init_task();
    scheduler_.add_timer_queue(timer_queue_);
  }

  // Destructor.
  ~deadline_timer_service()
  {
    scheduler_.remove_timer_queue(timer_queue_);
  }

  // Destroy all user-defined handler objects owned by the service.
  void shutdown()
  {
  }

  // Construct a new timer implementation.
  void construct(implementation_type& impl)
  {
    impl.expiry = time_type();
    impl.might_have_pending_waits = false;
  }

  // Destroy a timer implementation.
  void destroy(implementation_type& impl)
  {
    std::error_code ec;
    cancel(impl, ec);
  }

  // Move-construct a new serial port implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
    scheduler_.move_timer(timer_queue_, impl.timer_data, other_impl.timer_data);

    impl.expiry = other_impl.expiry;
    other_impl.expiry = time_type();

    impl.might_have_pending_waits = other_impl.might_have_pending_waits;
    other_impl.might_have_pending_waits = false;
  }

  // Move-assign from another serial port implementation.
  void move_assign(implementation_type& impl,
      deadline_timer_service& other_service,
      implementation_type& other_impl)
  {
    if (this != &other_service)
      if (impl.might_have_pending_waits)
        scheduler_.cancel_timer(timer_queue_, impl.timer_data);

    other_service.scheduler_.move_timer(other_service.timer_queue_,
        impl.timer_data, other_impl.timer_data);

    impl.expiry = other_impl.expiry;
    other_impl.expiry = time_type();

    impl.might_have_pending_waits = other_impl.might_have_pending_waits;
    other_impl.might_have_pending_waits = false;
  }

  // Cancel any asynchronous wait operations associated with the timer.
  std::size_t cancel(implementation_type& impl, std::error_code& ec)
  {
    if (!impl.might_have_pending_waits)
    {
      ec = std::error_code();
      return 0;
    }

    NET_TS_HANDLER_OPERATION((scheduler_.context(),
          "deadline_timer", &impl, 0, "cancel"));

    std::size_t count = scheduler_.cancel_timer(timer_queue_, impl.timer_data);
    impl.might_have_pending_waits = false;
    ec = std::error_code();
    return count;
  }

  // Cancels one asynchronous wait operation associated with the timer.
  std::size_t cancel_one(implementation_type& impl,
      std::error_code& ec)
  {
    if (!impl.might_have_pending_waits)
    {
      ec = std::error_code();
      return 0;
    }

    NET_TS_HANDLER_OPERATION((scheduler_.context(),
          "deadline_timer", &impl, 0, "cancel_one"));

    std::size_t count = scheduler_.cancel_timer(
        timer_queue_, impl.timer_data, 1);
    if (count == 0)
      impl.might_have_pending_waits = false;
    ec = std::error_code();
    return count;
  }

  // Get the expiry time for the timer as an absolute time.
  time_type expiry(const implementation_type& impl) const
  {
    return impl.expiry;
  }

  // Get the expiry time for the timer as an absolute time.
  time_type expires_at(const implementation_type& impl) const
  {
    return impl.expiry;
  }

  // Get the expiry time for the timer relative to now.
  duration_type expires_from_now(const implementation_type& impl) const
  {
    return Time_Traits::subtract(this->expiry(impl), Time_Traits::now());
  }

  // Set the expiry time for the timer as an absolute time.
  std::size_t expires_at(implementation_type& impl,
      const time_type& expiry_time, std::error_code& ec)
  {
    std::size_t count = cancel(impl, ec);
    impl.expiry = expiry_time;
    ec = std::error_code();
    return count;
  }

  // Set the expiry time for the timer relative to now.
  std::size_t expires_after(implementation_type& impl,
      const duration_type& expiry_time, std::error_code& ec)
  {
    return expires_at(impl,
        Time_Traits::add(Time_Traits::now(), expiry_time), ec);
  }

  // Set the expiry time for the timer relative to now.
  std::size_t expires_from_now(implementation_type& impl,
      const duration_type& expiry_time, std::error_code& ec)
  {
    return expires_at(impl,
        Time_Traits::add(Time_Traits::now(), expiry_time), ec);
  }

  // Perform a blocking wait on the timer.
  void wait(implementation_type& impl, std::error_code& ec)
  {
    time_type now = Time_Traits::now();
    ec = std::error_code();
    while (Time_Traits::less_than(now, impl.expiry) && !ec)
    {
      this->do_wait(Time_Traits::to_posix_duration(
            Time_Traits::subtract(impl.expiry, now)), ec);
      now = Time_Traits::now();
    }
  }

  // Start an asynchronous wait on the timer.
  template <typename Handler>
  void async_wait(implementation_type& impl, Handler& handler)
  {
    // Allocate and construct an operation to wrap the handler.
    typedef wait_handler<Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(handler);

    impl.might_have_pending_waits = true;

    NET_TS_HANDLER_CREATION((scheduler_.context(),
          *p.p, "deadline_timer", &impl, 0, "async_wait"));

    scheduler_.schedule_timer(timer_queue_, impl.expiry, impl.timer_data, p.p);
    p.v = p.p = 0;
  }

private:
  // Helper function to wait given a duration type. The duration type should
  // either be of type boost::posix_time::time_duration, or implement the
  // required subset of its interface.
  template <typename Duration>
  void do_wait(const Duration& timeout, std::error_code& ec)
  {
#if defined(NET_TS_WINDOWS_RUNTIME)
    std::this_thread::sleep_for(
        std::chrono::seconds(timeout.total_seconds())
        + std::chrono::microseconds(timeout.total_microseconds()));
    ec = std::error_code();
#else // defined(NET_TS_WINDOWS_RUNTIME)
    ::timeval tv;
    tv.tv_sec = timeout.total_seconds();
    tv.tv_usec = timeout.total_microseconds() % 1000000;
    socket_ops::select(0, 0, 0, 0, &tv, ec);
#endif // defined(NET_TS_WINDOWS_RUNTIME)
  }

  // The queue of timers.
  timer_queue<Time_Traits> timer_queue_;

  // The object that schedules and executes timers. Usually a reactor.
  timer_scheduler& scheduler_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_DEADLINE_TIMER_SERVICE_HPP
