//
// detail/reactive_socket_accept_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_REACTIVE_SOCKET_ACCEPT_OP_HPP
#define NET_TS_DETAIL_REACTIVE_SOCKET_ACCEPT_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/bind_handler.hpp>
#include <experimental/__net_ts/detail/buffer_sequence_adapter.hpp>
#include <experimental/__net_ts/detail/fenced_block.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/detail/reactor_op.hpp>
#include <experimental/__net_ts/detail/socket_holder.hpp>
#include <experimental/__net_ts/detail/socket_ops.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename Socket, typename Protocol>
class reactive_socket_accept_op_base : public reactor_op
{
public:
  reactive_socket_accept_op_base(socket_type socket,
      socket_ops::state_type state, Socket& peer, const Protocol& protocol,
      typename Protocol::endpoint* peer_endpoint, func_type complete_func)
    : reactor_op(&reactive_socket_accept_op_base::do_perform, complete_func),
      socket_(socket),
      state_(state),
      peer_(peer),
      protocol_(protocol),
      peer_endpoint_(peer_endpoint),
      addrlen_(peer_endpoint ? peer_endpoint->capacity() : 0)
  {
  }

  static status do_perform(reactor_op* base)
  {
    reactive_socket_accept_op_base* o(
        static_cast<reactive_socket_accept_op_base*>(base));

    socket_type new_socket = invalid_socket;
    status result = socket_ops::non_blocking_accept(o->socket_,
        o->state_, o->peer_endpoint_ ? o->peer_endpoint_->data() : 0,
        o->peer_endpoint_ ? &o->addrlen_ : 0, o->ec_, new_socket)
    ? done : not_done;
    o->new_socket_.reset(new_socket);

    NET_TS_HANDLER_REACTOR_OPERATION((*o, "non_blocking_accept", o->ec_));

    return result;
  }

  void do_assign()
  {
    if (new_socket_.get() != invalid_socket)
    {
      if (peer_endpoint_)
        peer_endpoint_->resize(addrlen_);
      peer_.assign(protocol_, new_socket_.get(), ec_);
      if (!ec_)
        new_socket_.release();
    }
  }

private:
  socket_type socket_;
  socket_ops::state_type state_;
  socket_holder new_socket_;
  Socket& peer_;
  Protocol protocol_;
  typename Protocol::endpoint* peer_endpoint_;
  std::size_t addrlen_;
};

template <typename Socket, typename Protocol, typename Handler>
class reactive_socket_accept_op :
  public reactive_socket_accept_op_base<Socket, Protocol>
{
public:
  NET_TS_DEFINE_HANDLER_PTR(reactive_socket_accept_op);

  reactive_socket_accept_op(socket_type socket,
      socket_ops::state_type state, Socket& peer, const Protocol& protocol,
      typename Protocol::endpoint* peer_endpoint, Handler& handler)
    : reactive_socket_accept_op_base<Socket, Protocol>(socket, state, peer,
        protocol, peer_endpoint, &reactive_socket_accept_op::do_complete),
      handler_(NET_TS_MOVE_CAST(Handler)(handler))
  {
    handler_work<Handler>::start(handler_);
  }

  static void do_complete(void* owner, operation* base,
      const std::error_code& /*ec*/,
      std::size_t /*bytes_transferred*/)
  {
    // Take ownership of the handler object.
    reactive_socket_accept_op* o(static_cast<reactive_socket_accept_op*>(base));
    ptr p = { std::experimental::net::v1::detail::addressof(o->handler_), o, o };
    handler_work<Handler> w(o->handler_);

    // On success, assign new connection to peer socket object.
    if (owner)
      o->do_assign();

    NET_TS_HANDLER_COMPLETION((*o));

    // Make a copy of the handler so that the memory can be deallocated before
    // the upcall is made. Even if we're not about to make an upcall, a
    // sub-object of the handler may be the true owner of the memory associated
    // with the handler. Consequently, a local copy of the handler is required
    // to ensure that any owning sub-object remains valid until after we have
    // deallocated the memory here.
    detail::binder1<Handler, std::error_code>
      handler(o->handler_, o->ec_);
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
  Handler handler_;
};

#if defined(NET_TS_HAS_MOVE)

template <typename Protocol, typename Handler>
class reactive_socket_move_accept_op :
  private Protocol::socket,
  public reactive_socket_accept_op_base<typename Protocol::socket, Protocol>
{
public:
  NET_TS_DEFINE_HANDLER_PTR(reactive_socket_move_accept_op);

  reactive_socket_move_accept_op(io_context& ioc, socket_type socket,
      socket_ops::state_type state, const Protocol& protocol,
      typename Protocol::endpoint* peer_endpoint, Handler& handler)
    : Protocol::socket(ioc),
      reactive_socket_accept_op_base<typename Protocol::socket, Protocol>(
        socket, state, *this, protocol, peer_endpoint,
        &reactive_socket_move_accept_op::do_complete),
      handler_(NET_TS_MOVE_CAST(Handler)(handler))
  {
    handler_work<Handler>::start(handler_);
  }

  static void do_complete(void* owner, operation* base,
      const std::error_code& /*ec*/,
      std::size_t /*bytes_transferred*/)
  {
    // Take ownership of the handler object.
    reactive_socket_move_accept_op* o(
        static_cast<reactive_socket_move_accept_op*>(base));
    ptr p = { std::experimental::net::v1::detail::addressof(o->handler_), o, o };
    handler_work<Handler> w(o->handler_);

    // On success, assign new connection to peer socket object.
    if (owner)
      o->do_assign();

    NET_TS_HANDLER_COMPLETION((*o));

    // Make a copy of the handler so that the memory can be deallocated before
    // the upcall is made. Even if we're not about to make an upcall, a
    // sub-object of the handler may be the true owner of the memory associated
    // with the handler. Consequently, a local copy of the handler is required
    // to ensure that any owning sub-object remains valid until after we have
    // deallocated the memory here.
    detail::move_binder2<Handler,
      std::error_code, typename Protocol::socket>
        handler(0, NET_TS_MOVE_CAST(Handler)(o->handler_), o->ec_,
          NET_TS_MOVE_CAST(typename Protocol::socket)(*o));
    p.h = std::experimental::net::v1::detail::addressof(handler.handler_);
    p.reset();

    // Make the upcall if required.
    if (owner)
    {
      fenced_block b(fenced_block::half);
      NET_TS_HANDLER_INVOCATION_BEGIN((handler.arg1_, "..."));
      w.complete(handler, handler.handler_);
      NET_TS_HANDLER_INVOCATION_END;
    }
  }

private:
  Handler handler_;
};

#endif // defined(NET_TS_HAS_MOVE)

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_REACTIVE_SOCKET_ACCEPT_OP_HPP
