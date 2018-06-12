//
// impl/dispatch.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IMPL_DISPATCH_HPP
#define NET_TS_IMPL_DISPATCH_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/associated_allocator.hpp>
#include <experimental/__net_ts/associated_executor.hpp>
#include <experimental/__net_ts/detail/work_dispatcher.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

template <typename CompletionToken>
NET_TS_INITFN_RESULT_TYPE(CompletionToken, void()) dispatch(
    NET_TS_MOVE_ARG(CompletionToken) token)
{
  typedef NET_TS_HANDLER_TYPE(CompletionToken, void()) handler;

  async_completion<CompletionToken, void()> init(token);

  typename associated_executor<handler>::type ex(
      (get_associated_executor)(init.completion_handler));

  typename associated_allocator<handler>::type alloc(
      (get_associated_allocator)(init.completion_handler));

  ex.dispatch(NET_TS_MOVE_CAST(handler)(init.completion_handler), alloc);

  return init.result.get();
}

template <typename Executor, typename CompletionToken>
NET_TS_INITFN_RESULT_TYPE(CompletionToken, void()) dispatch(
    const Executor& ex, NET_TS_MOVE_ARG(CompletionToken) token,
    typename enable_if<is_executor<Executor>::value>::type*)
{
  typedef NET_TS_HANDLER_TYPE(CompletionToken, void()) handler;

  async_completion<CompletionToken, void()> init(token);

  typename associated_allocator<handler>::type alloc(
      (get_associated_allocator)(init.completion_handler));

  ex.dispatch(detail::work_dispatcher<handler>(
        init.completion_handler), alloc);

  return init.result.get();
}

template <typename ExecutionContext, typename CompletionToken>
inline NET_TS_INITFN_RESULT_TYPE(CompletionToken, void()) dispatch(
    ExecutionContext& ctx, NET_TS_MOVE_ARG(CompletionToken) token,
    typename enable_if<is_convertible<
      ExecutionContext&, execution_context&>::value>::type*)
{
  return (dispatch)(ctx.get_executor(),
      NET_TS_MOVE_CAST(CompletionToken)(token));
}

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IMPL_DISPATCH_HPP
