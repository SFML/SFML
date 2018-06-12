//
// detail/handler_work.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_HANDLER_WORK_HPP
#define NET_TS_DETAIL_HANDLER_WORK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/associated_executor.hpp>
#include <experimental/__net_ts/detail/handler_invoke_helpers.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

// A helper class template to allow completion handlers to be dispatched
// through either the new executors framework or the old invocaton hook. The
// primary template uses the new executors framework.
template <typename Handler, typename Executor
    = typename associated_executor<Handler>::type>
class handler_work
{
public:
  explicit handler_work(Handler& handler) NET_TS_NOEXCEPT
    : executor_(associated_executor<Handler>::get(handler))
  {
  }

  static void start(Handler& handler) NET_TS_NOEXCEPT
  {
    Executor ex(associated_executor<Handler>::get(handler));
    ex.on_work_started();
  }

  ~handler_work()
  {
    executor_.on_work_finished();
  }

  template <typename Function>
  void complete(Function& function, Handler& handler)
  {
    executor_.dispatch(NET_TS_MOVE_CAST(Function)(function),
        associated_allocator<Handler>::get(handler));
  }

private:
  // Disallow copying and assignment.
  handler_work(const handler_work&);
  handler_work& operator=(const handler_work&);

  typename associated_executor<Handler>::type executor_;
};

// This specialisation dispatches a handler through the old invocation hook.
// The specialisation is not strictly required for correctness, as the
// system_executor will dispatch through the hook anyway. However, by doing
// this we avoid an extra copy of the handler.
template <typename Handler>
class handler_work<Handler, system_executor>
{
public:
  explicit handler_work(Handler&) NET_TS_NOEXCEPT {}
  static void start(Handler&) NET_TS_NOEXCEPT {}
  ~handler_work() {}

  template <typename Function>
  void complete(Function& function, Handler& handler)
  {
    networking_ts_handler_invoke_helpers::invoke(function, handler);
  }

private:
  // Disallow copying and assignment.
  handler_work(const handler_work&);
  handler_work& operator=(const handler_work&);
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_HANDLER_WORK_HPP
