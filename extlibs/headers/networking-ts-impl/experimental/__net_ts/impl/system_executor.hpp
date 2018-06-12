//
// impl/system_executor.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IMPL_SYSTEM_EXECUTOR_HPP
#define NET_TS_IMPL_SYSTEM_EXECUTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/executor_op.hpp>
#include <experimental/__net_ts/detail/global.hpp>
#include <experimental/__net_ts/detail/recycling_allocator.hpp>
#include <experimental/__net_ts/detail/type_traits.hpp>
#include <experimental/__net_ts/system_context.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

inline system_context& system_executor::context() const NET_TS_NOEXCEPT
{
  return detail::global<system_context>();
}

template <typename Function, typename Allocator>
void system_executor::dispatch(
    NET_TS_MOVE_ARG(Function) f, const Allocator&) const
{
  typename decay<Function>::type tmp(NET_TS_MOVE_CAST(Function)(f));
  networking_ts_handler_invoke_helpers::invoke(tmp, tmp);
}

template <typename Function, typename Allocator>
void system_executor::post(
    NET_TS_MOVE_ARG(Function) f, const Allocator& a) const
{
  typedef typename decay<Function>::type function_type;

  system_context& ctx = detail::global<system_context>();

  // Allocate and construct an operation to wrap the function.
  typedef detail::executor_op<function_type, Allocator> op;
  typename op::ptr p = { detail::addressof(a), op::ptr::allocate(a), 0 };
  p.p = new (p.v) op(NET_TS_MOVE_CAST(Function)(f), a);

  NET_TS_HANDLER_CREATION((ctx, *p.p,
        "system_executor", &this->context(), 0, "post"));

  ctx.scheduler_.post_immediate_completion(p.p, false);
  p.v = p.p = 0;
}

template <typename Function, typename Allocator>
void system_executor::defer(
    NET_TS_MOVE_ARG(Function) f, const Allocator& a) const
{
  typedef typename decay<Function>::type function_type;

  system_context& ctx = detail::global<system_context>();

  // Allocate and construct an operation to wrap the function.
  typedef detail::executor_op<function_type, Allocator> op;
  typename op::ptr p = { detail::addressof(a), op::ptr::allocate(a), 0 };
  p.p = new (p.v) op(NET_TS_MOVE_CAST(Function)(f), a);

  NET_TS_HANDLER_CREATION((ctx, *p.p,
        "system_executor", &this->context(), 0, "defer"));

  ctx.scheduler_.post_immediate_completion(p.p, true);
  p.v = p.p = 0;
}

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IMPL_SYSTEM_EXECUTOR_HPP
