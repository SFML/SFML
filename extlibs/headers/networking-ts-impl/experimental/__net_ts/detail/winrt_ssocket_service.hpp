//
// detail/winrt_ssocket_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_WINRT_SSOCKET_SERVICE_HPP
#define NET_TS_DETAIL_WINRT_SSOCKET_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS_RUNTIME)

#include <experimental/__net_ts/error.hpp>
#include <experimental/__net_ts/io_context.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/detail/winrt_socket_connect_op.hpp>
#include <experimental/__net_ts/detail/winrt_ssocket_service_base.hpp>
#include <experimental/__net_ts/detail/winrt_utils.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename Protocol>
class winrt_ssocket_service :
  public service_base<winrt_ssocket_service<Protocol> >,
  public winrt_ssocket_service_base
{
public:
  // The protocol type.
  typedef Protocol protocol_type;

  // The endpoint type.
  typedef typename Protocol::endpoint endpoint_type;

  // The native type of a socket.
  typedef Windows::Networking::Sockets::StreamSocket^ native_handle_type;

  // The implementation type of the socket.
  struct implementation_type : base_implementation_type
  {
    // Default constructor.
    implementation_type()
      : base_implementation_type(),
        protocol_(endpoint_type().protocol())
    {
    }

    // The protocol associated with the socket.
    protocol_type protocol_;
  };

  // Constructor.
  winrt_ssocket_service(std::experimental::net::v1::io_context& io_context)
    : service_base<winrt_ssocket_service<Protocol> >(io_context),
      winrt_ssocket_service_base(io_context)
  {
  }

  // Destroy all user-defined handler objects owned by the service.
  void shutdown()
  {
    this->base_shutdown();
  }

  // Move-construct a new socket implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
    this->base_move_construct(impl, other_impl);

    impl.protocol_ = other_impl.protocol_;
    other_impl.protocol_ = endpoint_type().protocol();
  }

  // Move-assign from another socket implementation.
  void move_assign(implementation_type& impl,
      winrt_ssocket_service& other_service,
      implementation_type& other_impl)
  {
    this->base_move_assign(impl, other_service, other_impl);

    impl.protocol_ = other_impl.protocol_;
    other_impl.protocol_ = endpoint_type().protocol();
  }

  // Move-construct a new socket implementation from another protocol type.
  template <typename Protocol1>
  void converting_move_construct(implementation_type& impl,
      winrt_ssocket_service<Protocol1>&,
      typename winrt_ssocket_service<
        Protocol1>::implementation_type& other_impl)
  {
    this->base_move_construct(impl, other_impl);

    impl.protocol_ = protocol_type(other_impl.protocol_);
    other_impl.protocol_ = typename Protocol1::endpoint().protocol();
  }

  // Open a new socket implementation.
  std::error_code open(implementation_type& impl,
      const protocol_type& protocol, std::error_code& ec)
  {
    if (is_open(impl))
    {
      ec = std::experimental::net::v1::error::already_open;
      return ec;
    }

    try
    {
      impl.socket_ = ref new Windows::Networking::Sockets::StreamSocket;
      impl.protocol_ = protocol;
      ec = std::error_code();
    }
    catch (Platform::Exception^ e)
    {
      ec = std::error_code(e->HResult,
            std::system_category());
    }

    return ec;
  }

  // Assign a native socket to a socket implementation.
  std::error_code assign(implementation_type& impl,
      const protocol_type& protocol, const native_handle_type& native_socket,
      std::error_code& ec)
  {
    if (is_open(impl))
    {
      ec = std::experimental::net::v1::error::already_open;
      return ec;
    }

    impl.socket_ = native_socket;
    impl.protocol_ = protocol;
    ec = std::error_code();

    return ec;
  }

  // Bind the socket to the specified local endpoint.
  std::error_code bind(implementation_type&,
      const endpoint_type&, std::error_code& ec)
  {
    ec = std::experimental::net::v1::error::operation_not_supported;
    return ec;
  }

  // Get the local endpoint.
  endpoint_type local_endpoint(const implementation_type& impl,
      std::error_code& ec) const
  {
    endpoint_type endpoint;
    endpoint.resize(do_get_endpoint(impl, true,
          endpoint.data(), endpoint.size(), ec));
    return endpoint;
  }

  // Get the remote endpoint.
  endpoint_type remote_endpoint(const implementation_type& impl,
      std::error_code& ec) const
  {
    endpoint_type endpoint;
    endpoint.resize(do_get_endpoint(impl, false,
          endpoint.data(), endpoint.size(), ec));
    return endpoint;
  }

  // Set a socket option.
  template <typename Option>
  std::error_code set_option(implementation_type& impl,
      const Option& option, std::error_code& ec)
  {
    return do_set_option(impl, option.level(impl.protocol_),
        option.name(impl.protocol_), option.data(impl.protocol_),
        option.size(impl.protocol_), ec);
  }

  // Get a socket option.
  template <typename Option>
  std::error_code get_option(const implementation_type& impl,
      Option& option, std::error_code& ec) const
  {
    std::size_t size = option.size(impl.protocol_);
    do_get_option(impl, option.level(impl.protocol_),
        option.name(impl.protocol_),
        option.data(impl.protocol_), &size, ec);
    if (!ec)
      option.resize(impl.protocol_, size);
    return ec;
  }

  // Connect the socket to the specified endpoint.
  std::error_code connect(implementation_type& impl,
      const endpoint_type& peer_endpoint, std::error_code& ec)
  {
    return do_connect(impl, peer_endpoint.data(), ec);
  }

  // Start an asynchronous connect.
  template <typename Handler>
  void async_connect(implementation_type& impl,
      const endpoint_type& peer_endpoint, Handler& handler)
  {
    bool is_continuation =
      networking_ts_handler_cont_helpers::is_continuation(handler);

    // Allocate and construct an operation to wrap the handler.
    typedef winrt_socket_connect_op<Handler> op;
    typename op::ptr p = { std::experimental::net::v1::detail::addressof(handler),
      op::ptr::allocate(handler), 0 };
    p.p = new (p.v) op(handler);

    NET_TS_HANDLER_CREATION((io_context_.context(),
          *p.p, "socket", &impl, 0, "async_connect"));

    start_connect_op(impl, peer_endpoint.data(), p.p, is_continuation);
    p.v = p.p = 0;
  }
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_WINRT_SSOCKET_SERVICE_HPP
