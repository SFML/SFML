//
// detail/winrt_socket_send_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_WINRT_SOCKET_SEND_OP_HPP
#define NET_TS_DETAIL_WINRT_SOCKET_SEND_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS_RUNTIME)

#include <experimental/__net_ts/detail/bind_handler.hpp>
#include <experimental/__net_ts/detail/buffer_sequence_adapter.hpp>
#include <experimental/__net_ts/detail/fenced_block.hpp>
#include <experimental/__net_ts/detail/handler_alloc_helpers.hpp>
#include <experimental/__net_ts/detail/handler_invoke_helpers.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/detail/winrt_async_op.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename ConstBufferSequence, typename Handler>
class winrt_socket_send_op :
  public winrt_async_op<unsigned int>
{
public:
  NET_TS_DEFINE_HANDLER_PTR(winrt_socket_send_op);

  winrt_socket_send_op(const ConstBufferSequence& buffers, Handler& handler)
    : winrt_async_op<unsigned int>(&winrt_socket_send_op::do_complete),
      buffers_(buffers),
      handler_(NET_TS_MOVE_CAST(Handler)(handler))
  {
    handler_work<Handler>::start(handler_);
  }

  static void do_complete(void* owner, operation* base,
      const std::error_code&, std::size_t)
  {
    // Take ownership of the operation object.
    winrt_socket_send_op* o(static_cast<winrt_socket_send_op*>(base));
    ptr p = { std::experimental::net::v1::detail::addressof(o->handler_), o, o };
    handler_work<Handler> w(o->handler_);

    NET_TS_HANDLER_COMPLETION((*o));

#if defined(NET_TS_ENABLE_BUFFER_DEBUGGING)
    // Check whether buffers are still valid.
    if (owner)
    {
      buffer_sequence_adapter<std::experimental::net::v1::const_buffer,
          ConstBufferSequence>::validate(o->buffers_);
    }
#endif // defined(NET_TS_ENABLE_BUFFER_DEBUGGING)

    // Make a copy of the handler so that the memory can be deallocated before
    // the upcall is made. Even if we're not about to make an upcall, a
    // sub-object of the handler may be the true owner of the memory associated
    // with the handler. Consequently, a local copy of the handler is required
    // to ensure that any owning sub-object remains valid until after we have
    // deallocated the memory here.
    detail::binder2<Handler, std::error_code, std::size_t>
      handler(o->handler_, o->ec_, o->result_);
    p.h = std::experimental::net::v1::detail::addressof(handler.handler_);
    p.reset();

    // Make the upcall if required.
    if (owner)
    {
      fenced_block b(fenced_block::half);
      NET_TS_HANDLER_INVOCATION_BEGIN((handler.arg1_, handler.arg2_));
      w.complete(handler, handler.handler_);
      NET_TS_HANDLER_INVOCATION_END;
    }
  }

private:
  ConstBufferSequence buffers_;
  Handler handler_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_WINRT_SOCKET_SEND_OP_HPP
