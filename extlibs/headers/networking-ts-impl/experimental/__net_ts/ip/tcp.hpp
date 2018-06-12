//
// ip/tcp.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_TCP_HPP
#define NET_TS_IP_TCP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/basic_socket_acceptor.hpp>
#include <experimental/__net_ts/basic_socket_iostream.hpp>
#include <experimental/__net_ts/basic_stream_socket.hpp>
#include <experimental/__net_ts/detail/socket_option.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>
#include <experimental/__net_ts/ip/basic_endpoint.hpp>
#include <experimental/__net_ts/ip/basic_resolver.hpp>
#include <experimental/__net_ts/ip/basic_resolver_iterator.hpp>
#include <experimental/__net_ts/ip/basic_resolver_query.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

/// Encapsulates the flags needed for TCP.
/**
 * The std::experimental::net::v1::ip::tcp class contains flags necessary for TCP sockets.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Safe.
 *
 * @par Concepts:
 * Protocol, InternetProtocol.
 */
class tcp
{
public:
  /// The type of a TCP endpoint.
  typedef basic_endpoint<tcp> endpoint;

  /// Construct to represent the IPv4 TCP protocol.
  static tcp v4()
  {
    return tcp(NET_TS_OS_DEF(AF_INET));
  }

  /// Construct to represent the IPv6 TCP protocol.
  static tcp v6()
  {
    return tcp(NET_TS_OS_DEF(AF_INET6));
  }

  /// Obtain an identifier for the type of the protocol.
  int type() const
  {
    return NET_TS_OS_DEF(SOCK_STREAM);
  }

  /// Obtain an identifier for the protocol.
  int protocol() const
  {
    return NET_TS_OS_DEF(IPPROTO_TCP);
  }

  /// Obtain an identifier for the protocol family.
  int family() const
  {
    return family_;
  }

  /// The TCP socket type.
  typedef basic_stream_socket<tcp> socket;

  /// The TCP acceptor type.
  typedef basic_socket_acceptor<tcp> acceptor;

  /// The TCP resolver type.
  typedef basic_resolver<tcp> resolver;

#if !defined(NET_TS_NO_IOSTREAM)
  /// The TCP iostream type.
  typedef basic_socket_iostream<tcp> iostream;
#endif // !defined(NET_TS_NO_IOSTREAM)

  /// Socket option for disabling the Nagle algorithm.
  /**
   * Implements the IPPROTO_TCP/TCP_NODELAY socket option.
   *
   * @par Examples
   * Setting the option:
   * @code
   * std::experimental::net::ip::tcp::socket socket(io_context); 
   * ...
   * std::experimental::net::ip::tcp::no_delay option(true);
   * socket.set_option(option);
   * @endcode
   *
   * @par
   * Getting the current option value:
   * @code
   * std::experimental::net::ip::tcp::socket socket(io_context); 
   * ...
   * std::experimental::net::ip::tcp::no_delay option;
   * socket.get_option(option);
   * bool is_set = option.value();
   * @endcode
   *
   * @par Concepts:
   * Socket_Option, Boolean_Socket_Option.
   */
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined no_delay;
#else
  typedef std::experimental::net::v1::detail::socket_option::boolean<
    NET_TS_OS_DEF(IPPROTO_TCP), NET_TS_OS_DEF(TCP_NODELAY)> no_delay;
#endif

  /// Compare two protocols for equality.
  friend bool operator==(const tcp& p1, const tcp& p2)
  {
    return p1.family_ == p2.family_;
  }

  /// Compare two protocols for inequality.
  friend bool operator!=(const tcp& p1, const tcp& p2)
  {
    return p1.family_ != p2.family_;
  }

private:
  // Construct with a specific family.
  explicit tcp(int protocol_family)
    : family_(protocol_family)
  {
  }

  int family_;
};

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IP_TCP_HPP
