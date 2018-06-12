//
// detail/win_iocp_wait_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_WIN_IOCP_WAIT_OP_HPP
#define NET_TS_DETAIL_WIN_IOCP_WAIT_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_IOCP)

#include <experimental/__net_ts/detail/bind_handler.hpp>
#include <experimental/__net_ts/detail/buffer_sequence_adapter.hpp>
#include <experimental/__net_ts/detail/fenced_block.hpp>
#include <experimental/__net_ts/detail/handler_alloc_helpers.hpp>
#include <experimental/__net_ts/detail/handler_invoke_helpers.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/detail/reactor_op.hpp>
#include <experimental/__net_ts/detail/socket_ops.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename Handler>
class win_iocp_wait_op : public reactor_op
{
public:
  NET_TS_DEFINE_HANDLER_PTR(win_iocp_wait_op);

  win_iocp_wait_op(socket_ops::weak_cancel_token_type cancel_token,
      Handler& handler)
    : reactor_op(&win_iocp_wait_op::do_perform,
        &win_iocp_wait_op::do_complete),
      cancel_token_(cancel_token),
      handler_(NET_TS_MOVE_CAST(Handler)(handler))
  {
    handler_work<Handler>::start(handler_);
  }

  static status do_perform(reactor_op*)
  {
    return done;
  }

  static void do_complete(void* owner, operation* base,
      const std::error_code& result_ec,
      std::size_t /*bytes_transferred*/)
  {
    std::error_code ec(result_ec);

    // Take ownership of the operation object.
    win_iocp_wait_op* o(static_cast<win_iocp_wait_op*>(base));
    ptr p = { std::experimental::net::v1::detail::addressof(o->handler_), o, o };
    handler_work<Handler> w(o->handler_);

    NET_TS_HANDLER_COMPLETION((*o));

    // The reactor may have stored a result in the operation object.
    if (o->ec_)
      ec = o->ec_;

    // Map non-portable errors to their portable counterparts.
    if (ec.value() == ERROR_NETNAME_DELETED)
    {
      if (o->cancel_token_.expired())
        ec = std::experimental::net::v1::error::operation_aborted;
      else
        ec = std::experimental::net::v1::error::connection_reset;
    }
    else if (ec.value() == ERROR_PORT_UNREACHABLE)
    {
      ec = std::experimental::net::v1::error::connection_refused;
    }

    // Make a copy of the handler so that the memory can be deallocated before
    // the upcall is made. Even if we're not about to make an upcall, a
    // sub-object of the handler may be the true owner of the memory associated
    // with the handler. Consequently, a local copy of the handler is required
    // to ensure that any owning sub-object remains valid until after we have
    // deallocated the memory here.
    detail::binder1<Handler, std::error_code>
      handler(o->handler_, ec);
    p.h = std::experimental::net::v1::detail::addressof(handler.handler_);
    p.reset();

    // Make the upcall if required.
    if (owner)
    {
      fenced_block b(fenced_block::half);
      NET_TS_HANDLER_INVOCATION_BEGIN((handler.arg1_));
      w.complete(handler, handler.handler_);
      NET_TS_HANDLER_INVOCATION_END;
    }
  }

private:
  socket_ops::weak_cancel_token_type cancel_token_;
  Handler handler_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_HAS_IOCP)

#endif // NET_TS_DETAIL_WIN_IOCP_WAIT_OP_HPP
