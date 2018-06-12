//
// detail/impl/winrt_timer_scheduler.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_WINRT_TIMER_SCHEDULER_IPP
#define NET_TS_DETAIL_IMPL_WINRT_TIMER_SCHEDULER_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS_RUNTIME)

#include <experimental/__net_ts/detail/bind_handler.hpp>
#include <experimental/__net_ts/detail/winrt_timer_scheduler.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

winrt_timer_scheduler::winrt_timer_scheduler(
    std::experimental::net::v1::io_context& io_context)
  : std::experimental::net::v1::detail::service_base<winrt_timer_scheduler>(io_context),
    io_context_(use_service<io_context_impl>(io_context)),
    mutex_(),
    event_(),
    timer_queues_(),
    thread_(0),
    stop_thread_(false),
    shutdown_(false)
{
  thread_ = new std::experimental::net::v1::detail::thread(
      bind_handler(&winrt_timer_scheduler::call_run_thread, this));
}

winrt_timer_scheduler::~winrt_timer_scheduler()
{
  shutdown();
}

void winrt_timer_scheduler::shutdown()
{
  std::experimental::net::v1::detail::mutex::scoped_lock lock(mutex_);
  shutdown_ = true;
  stop_thread_ = true;
  event_.signal(lock);
  lock.unlock();

  if (thread_)
  {
    thread_->join();
    delete thread_;
    thread_ = 0;
  }

  op_queue<operation> ops;
  timer_queues_.get_all_timers(ops);
  io_context_.abandon_operations(ops);
}

void winrt_timer_scheduler::notify_fork(std::experimental::net::v1::io_context::fork_event)
{
}

void winrt_timer_scheduler::init_task()
{
}

void winrt_timer_scheduler::run_thread()
{
  std::experimental::net::v1::detail::mutex::scoped_lock lock(mutex_);
  while (!stop_thread_)
  {
    const long max_wait_duration = 5 * 60 * 1000000;
    long wait_duration = timer_queues_.wait_duration_usec(max_wait_duration);
    event_.wait_for_usec(lock, wait_duration);
    event_.clear(lock);
    op_queue<operation> ops;
    timer_queues_.get_ready_timers(ops);
    if (!ops.empty())
    {
      lock.unlock();
      io_context_.post_deferred_completions(ops);
      lock.lock();
    }
  }
}

void winrt_timer_scheduler::call_run_thread(winrt_timer_scheduler* scheduler)
{
  scheduler->run_thread();
}

void winrt_timer_scheduler::do_add_timer_queue(timer_queue_base& queue)
{
  mutex::scoped_lock lock(mutex_);
  timer_queues_.insert(&queue);
}

void winrt_timer_scheduler::do_remove_timer_queue(timer_queue_base& queue)
{
  mutex::scoped_lock lock(mutex_);
  timer_queues_.erase(&queue);
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_IMPL_WINRT_TIMER_SCHEDULER_IPP
