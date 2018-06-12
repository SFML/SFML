//
// detail/reactive_socket_service_base.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_REACTIVE_SOCKET_SERVICE_BASE_HPP
#define NET_TS_DETAIL_REACTIVE_SOCKET_SERVICE_BASE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_IOCP) \
  && !defined(NET_TS_WINDOWS_RUNTIME)

#include <experimental/__net_ts/buffer.hpp>
#include <experimental/__net_ts/error.hpp>
#include <experimental/__net_ts/io_context.hpp>
#include <experimental/__net_ts/socket_base.hpp>
#include <experimental/__net_ts/detail/buffer_sequence_adapter.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/detail/reactive_null_buffers_op.hpp>
#include <experimental/__net_ts/detail/reactive_socket_recv_op.hpp>
#include <experimental/__net_ts/detail/reactive_socket_recvmsg_op.hpp>
#include <experimental/__net_ts/detail/reactive_socket_send_op.hpp>
#include <experimental/__net_ts/detail/reactive_wait_op.hpp>
#include <experimental/__net_ts/detail/reactor.hpp>
#include <experimental/__net_ts/detail/reactor_op.hpp>
#include <experimental/__net_ts/detail/socket_holder.hpp>
#include <experimental/__net_ts/detail/socket_ops.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class reactive_socket_service_base
{
public:
  // The native type of a socket.
  typedef socket_type native_handle_type;

  // The implementation type of the socket.
  struct base_implementation_type
  {
    // The native socket representation.
    socket_type socket_;

    // The current state of the socket.
    socket_ops::state_type state_;

    // Per-descriptor data used by the reactor.
    reactor::per_descriptor_data reactor_data_;
  };

  // Constructor.
  NET_TS_DECL reactive_socket_service_base(
      std::experimental::net::v1::io_context& io_context);

  // Destroy all user-defined handler objects owned by the service.
  NET_TS_DECL void base_shutdown();

  // Construct a new socket implementation.
  NET_TS_DECL void construct(base_implementation_type& impl);

  // Move-construct a new socket implementation.
  NET_TS_DECL void base_move_construct(base_implementation_type& impl,
      base_implementation_type& other_impl);

  // Move-assign from another socket implementation.
  NET_TS_DECL void base_move_assign(base_implementation_type& impl,
      reactive_socket_service_base& other_service,
      base_implementation_type& other_impl);

  // Destroy a socket implementation.
  NET_TS_DECL void destroy(base_implementation_type& impl);

  // Determine whether the socket is open.
  bool is_open(const base_implementation_type& impl) const
  {
    return impl.socket_ != invalid_socket;
  }

  // Destroy a socket implementation.
  NET_TS_DECL std::error_code close(
      base_implementation_type& impl, std::error_code& ec);

  // Release ownership of the socket.
  NET_TS_DECL socket_type release(
      base_implementation_type& impl, std::error_code& ec);

  // Get the native socket representation.
  native_handle_type native_handle(base_implementation_type& impl)
  {
    return impl.socket_;
  }

  // Cancel all operations associated with the socket.
  NET_TS_DECL std::error_code cancel(
      base_implementation_type& impl, std::error_code& ec);

  // Determine whether the socket is at the out-of-band data mark.
  bool at_mark(const base_implementation_type& impl,
      std::error_code& ec) const
  {
    return socket_ops::sockatmark(impl.socket_, ec);
  }

  // Determine the number of bytes available for reading.
  std::size_t available(const base_implementation_type& impl,
      std::error_code& ec) const
  {
    return socket_ops::available(impl.socket_, ec);
  }

  // Place the socket into the state where it will listen for new connections.
  std::error_code listen(base_implementation_type& impl,
      int backlog, std::error_code& ec)
  {
    socket_ops::listen(impl.socket_, backlog, ec);
    return ec;
  }

  // Perform an IO control command on the socket.
  template <typename IO_Control_Command>
  std::error_code io_control(base_implementation_type& impl,
      IO_Control_Command& command, std::error_code& ec)
  {
    socket_ops::ioctl(impl.socket_, impl.state_, command.name(),
        static_cast<ioctl_arg_type*>(command.data()), ec);
    return ec;
  }

  // Gets the non-blocking mode of the socket.
  bool non_blocking(const base_implementation_type& impl) const
  {
    return (impl.state_ & socket_ops::user_set_non_blocking) != 0;
  }

  // Sets the non-blocking mode of the socket.
  std::error_code non_blocking(base_implementation_type& impl,
      bool mode, std::error_code& ec)
  {
    socket_ops::set_user_non_blocking(impl.socket_, impl.state_, mode, ec);
    return ec;
  }

  // Gets the non-blocking mode of the native socket implementation.
  bool native_non_blocking(const base_implementation_type& impl) const
  {
    return (impl.state_ & socket_ops::internal_non_blocking) != 0;
  }

  // Sets the non-blocking mode of the native socket implementation.
  std::error_code native_non_blocking(base_implementation_type& impl,
      bool mode, std::error_code& ec)
  {
    socket_ops::set_internal_non_blocking(impl.socket_, impl.state_, mode, ec);
    return ec;
  }

  // Wait for the socket to become ready to read, ready to write, or to have
  // pending error conditions.
  std::error_code wait(base_implementation_type& impl,
      socket_base::wait_type w, std::error_code& ec)
  {
    switch (w)
    {
    case socket_base::wait_read:
      socket_ops::poll_read(impl.socket_, impl.state_, -1, ec);
      break;
    case socket_base::wait_write:
      socket_ops::poll_write(impl.socket_, impl.state_, -1, ec);
      break;
    case socket_base::wait_error:
      socket_ops::poll_error(impl.socket_, impl.state_, -1, ec);
      break;
    default:
      ec = std::experimental::net::v1::error::invalid_argument;
      break;
    }

    return ec;
  }

  // Asynchronously wait for the socket to become ready to read, ready to
  // write, or to have pending error conditions.
  template <typename Handler>
  void async_wait(base_implementation_type& impl,
      socket_base::wait_type w, Handler& handler)
  {
    bool is_continuation =
      networking_ts_handler_cont_helpers::is_continuation(handler);

    // Allocate and construct an operation to wrap the handler.
    typedef reactive_wait_op<Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(handler);

    NET_TS_HANDLER_CREATION((reactor_.context(), *p.p, "socket",
          &impl, impl.socket_, "async_wait"));

    int op_type;
    switch (w)
    {
      case socket_base::wait_read:
        op_type = reactor::read_op;
        break;
      case socket_base::wait_write:
        op_type = reactor::write_op;
        break;
      case socket_base::wait_error:
        op_type = reactor::except_op;
        break;
      default:
        p.p->ec_ = std::experimental::net::v1::error::invalid_argument;
        reactor_.post_immediate_completion(p.p, is_continuation);
        p.v = p.p = 0;
        return;
    }

    start_op(impl, op_type, p.p, is_continuation, false, false);
    p.v = p.p = 0;
  }

  // Send the given data to the peer.
  template <typename ConstBufferSequence>
  size_t send(base_implementation_type& impl,
      const ConstBufferSequence& buffers,
      socket_base::message_flags flags, std::error_code& ec)
  {
    buffer_sequence_adapter<std::experimental::net::v1::const_buffer,
        ConstBufferSequence> bufs(buffers);

    return socket_ops::sync_send(impl.socket_, impl.state_,
        bufs.buffers(), bufs.count(), flags, bufs.all_empty(), ec);
  }

  // Wait until data can be sent without blocking.
  size_t send(base_implementation_type& impl, const null_buffers&,
      socket_base::message_flags, std::error_code& ec)
  {
    // Wait for socket to become ready.
    socket_ops::poll_write(impl.socket_, impl.state_, -1, ec);

    return 0;
  }

  // Start an asynchronous send. The data being sent must be valid for the
  // lifetime of the asynchronous operation.
  template <typename ConstBufferSequence, typename Handler>
  void async_send(base_implementation_type& impl,
      const ConstBufferSequence& buffers,
      socket_base::message_flags flags, Handler& handler)
  {
    bool is_continuation =
      networking_ts_handler_cont_helpers::is_continuation(handler);

    // Allocate and construct an operation to wrap the handler.
    typedef reactive_socket_send_op<ConstBufferSequence, Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(impl.socket_, impl.state_, buffers, flags, handler);

    NET_TS_HANDLER_CREATION((reactor_.context(), *p.p, "socket",
          &impl, impl.socket_, "async_send"));

    start_op(impl, reactor::write_op, p.p, is_continuation, true,
        ((impl.state_ & socket_ops::stream_oriented)
          && buffer_sequence_adapter<std::experimental::net::v1::const_buffer,
            ConstBufferSequence>::all_empty(buffers)));
    p.v = p.p = 0;
  }

  // Start an asynchronous wait until data can be sent without blocking.
  template <typename Handler>
  void async_send(base_implementation_type& impl, const null_buffers&,
      socket_base::message_flags, Handler& handler)
  {
    bool is_continuation =
      networking_ts_handler_cont_helpers::is_continuation(handler);

    // Allocate and construct an operation to wrap the handler.
    typedef reactive_null_buffers_op<Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(handler);

    NET_TS_HANDLER_CREATION((reactor_.context(), *p.p, "socket",
          &impl, impl.socket_, "async_send(null_buffers)"));

    start_op(impl, reactor::write_op, p.p, is_continuation, false, false);
    p.v = p.p = 0;
  }

  // Receive some data from the peer. Returns the number of bytes received.
  template <typename MutableBufferSequence>
  size_t receive(base_implementation_type& impl,
      const MutableBufferSequence& buffers,
      socket_base::message_flags flags, std::error_code& ec)
  {
    buffer_sequence_adapter<std::experimental::net::v1::mutable_buffer,
        MutableBufferSequence> bufs(buffers);

    return socket_ops::sync_recv(impl.socket_, impl.state_,
        bufs.buffers(), bufs.count(), flags, bufs.all_empty(), ec);
  }

  // Wait until data can be received without blocking.
  size_t receive(base_implementation_type& impl, const null_buffers&,
      socket_base::message_flags, std::error_code& ec)
  {
    // Wait for socket to become ready.
    socket_ops::poll_read(impl.socket_, impl.state_, -1, ec);

    return 0;
  }

  // Start an asynchronous receive. The buffer for the data being received
  // must be valid for the lifetime of the asynchronous operation.
  template <typename MutableBufferSequence, typename Handler>
  void async_receive(base_implementation_type& impl,
      const MutableBufferSequence& buffers,
      socket_base::message_flags flags, Handler& handler)
  {
    bool is_continuation =
      networking_ts_handler_cont_helpers::is_continuation(handler);

    // Allocate and construct an operation to wrap the handler.
    typedef reactive_socket_recv_op<MutableBufferSequence, Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(impl.socket_, impl.state_, buffers, flags, handler);

    NET_TS_HANDLER_CREATION((reactor_.context(), *p.p, "socket",
          &impl, impl.socket_, "async_receive"));

    start_op(impl,
        (flags & socket_base::message_out_of_band)
          ? reactor::except_op : reactor::read_op,
        p.p, is_continuation,
        (flags & socket_base::message_out_of_band) == 0,
        ((impl.state_ & socket_ops::stream_oriented)
          && buffer_sequence_adapter<std::experimental::net::v1::mutable_buffer,
            MutableBufferSequence>::all_empty(buffers)));
    p.v = p.p = 0;
  }

  // Wait until data can be received without blocking.
  template <typename Handler>
  void async_receive(base_implementation_type& impl, const null_buffers&,
      socket_base::message_flags flags, Handler& handler)
  {
    bool is_continuation =
      networking_ts_handler_cont_helpers::is_continuation(handler);

    // Allocate and construct an operation to wrap the handler.
    typedef reactive_null_buffers_op<Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(handler);

    NET_TS_HANDLER_CREATION((reactor_.context(), *p.p, "socket",
          &impl, impl.socket_, "async_receive(null_buffers)"));

    start_op(impl,
        (flags & socket_base::message_out_of_band)
          ? reactor::except_op : reactor::read_op,
        p.p, is_continuation, false, false);
    p.v = p.p = 0;
  }

  // Receive some data with associated flags. Returns the number of bytes
  // received.
  template <typename MutableBufferSequence>
  size_t receive_with_flags(base_implementation_type& impl,
      const MutableBufferSequence& buffers,
      socket_base::message_flags in_flags,
      socket_base::message_flags& out_flags, std::error_code& ec)
  {
    buffer_sequence_adapter<std::experimental::net::v1::mutable_buffer,
        MutableBufferSequence> bufs(buffers);

    return socket_ops::sync_recvmsg(impl.socket_, impl.state_,
        bufs.buffers(), bufs.count(), in_flags, out_flags, ec);
  }

  // Wait until data can be received without blocking.
  size_t receive_with_flags(base_implementation_type& impl,
      const null_buffers&, socket_base::message_flags,
      socket_base::message_flags& out_flags, std::error_code& ec)
  {
    // Wait for socket to become ready.
    socket_ops::poll_read(impl.socket_, impl.state_, -1, ec);

    // Clear out_flags, since we cannot give it any other sensible value when
    // performing a null_buffers operation.
    out_flags = 0;

    return 0;
  }

  // Start an asynchronous receive. The buffer for the data being received
  // must be valid for the lifetime of the asynchronous operation.
  template <typename MutableBufferSequence, typename Handler>
  void async_receive_with_flags(base_implementation_type& impl,
      const MutableBufferSequence& buffers, socket_base::message_flags in_flags,
      socket_base::message_flags& out_flags, Handler& handler)
  {
    bool is_continuation =
      networking_ts_handler_cont_helpers::is_continuation(handler);

    // Allocate and construct an operation to wrap the handler.
    typedef reactive_socket_recvmsg_op<MutableBufferSequence, Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(impl.socket_, buffers, in_flags, out_flags, handler);

    NET_TS_HANDLER_CREATION((reactor_.context(), *p.p, "socket",
          &impl, impl.socket_, "async_receive_with_flags"));

    start_op(impl,
        (in_flags & socket_base::message_out_of_band)
          ? reactor::except_op : reactor::read_op,
        p.p, is_continuation,
        (in_flags & socket_base::message_out_of_band) == 0, false);
    p.v = p.p = 0;
  }

  // Wait until data can be received without blocking.
  template <typename Handler>
  void async_receive_with_flags(base_implementation_type& impl,
      const null_buffers&, socket_base::message_flags in_flags,
      socket_base::message_flags& out_flags, Handler& handler)
  {
    bool is_continuation =
      networking_ts_handler_cont_helpers::is_continuation(handler);

    // Allocate and construct an operation to wrap the handler.
    typedef reactive_null_buffers_op<Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(handler);

    NET_TS_HANDLER_CREATION((reactor_.context(), *p.p, "socket",
          &impl, impl.socket_, "async_receive_with_flags(null_buffers)"));

    // Clear out_flags, since we cannot give it any other sensible value when
    // performing a null_buffers operation.
    out_flags = 0;

    start_op(impl,
        (in_flags & socket_base::message_out_of_band)
          ? reactor::except_op : reactor::read_op,
        p.p, is_continuation, false, false);
    p.v = p.p = 0;
  }

protected:
  // Open a new socket implementation.
  NET_TS_DECL std::error_code do_open(
      base_implementation_type& impl, int af,
      int type, int protocol, std::error_code& ec);

  // Assign a native socket to a socket implementation.
  NET_TS_DECL std::error_code do_assign(
      base_implementation_type& impl, int type,
      const native_handle_type& native_socket, std::error_code& ec);

  // Start the asynchronous read or write operation.
  NET_TS_DECL void start_op(base_implementation_type& impl, int op_type,
      reactor_op* op, bool is_continuation, bool is_non_blocking, bool noop);

  // Start the asynchronous accept operation.
  NET_TS_DECL void start_accept_op(base_implementation_type& impl,
      reactor_op* op, bool is_continuation, bool peer_is_open);

  // Start the asynchronous connect operation.
  NET_TS_DECL void start_connect_op(base_implementation_type& impl,
      reactor_op* op, bool is_continuation,
      const socket_addr_type* addr, size_t addrlen);

  // The io_context that owns this socket service.
  io_context& io_context_;

  // The selector that performs event demultiplexing for the service.
  reactor& reactor_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/reactive_socket_service_base.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // !defined(NET_TS_HAS_IOCP)
       //   && !defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_REACTIVE_SOCKET_SERVICE_BASE_HPP
