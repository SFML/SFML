//
// ip/udp.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_UDP_HPP
#define NET_TS_IP_UDP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/basic_datagram_socket.hpp>
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

/// Encapsulates the flags needed for UDP.
/**
 * The std::experimental::net::v1::ip::udp class contains flags necessary for UDP sockets.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Safe.
 *
 * @par Concepts:
 * Protocol, InternetProtocol.
 */
class udp
{
public:
  /// The type of a UDP endpoint.
  typedef basic_endpoint<udp> endpoint;

  /// Construct to represent the IPv4 UDP protocol.
  static udp v4()
  {
    return udp(NET_TS_OS_DEF(AF_INET));
  }

  /// Construct to represent the IPv6 UDP protocol.
  static udp v6()
  {
    return udp(NET_TS_OS_DEF(AF_INET6));
  }

  /// Obtain an identifier for the type of the protocol.
  int type() const
  {
    return NET_TS_OS_DEF(SOCK_DGRAM);
  }

  /// Obtain an identifier for the protocol.
  int protocol() const
  {
    return NET_TS_OS_DEF(IPPROTO_UDP);
  }

  /// Obtain an identifier for the protocol family.
  int family() const
  {
    return family_;
  }

  /// The UDP socket type.
  typedef basic_datagram_socket<udp> socket;

  /// The UDP resolver type.
  typedef basic_resolver<udp> resolver;

  /// Compare two protocols for equality.
  friend bool operator==(const udp& p1, const udp& p2)
  {
    return p1.family_ == p2.family_;
  }

  /// Compare two protocols for inequality.
  friend bool operator!=(const udp& p1, const udp& p2)
  {
    return p1.family_ != p2.family_;
  }

private:
  // Construct with a specific family.
  explicit udp(int protocol_family)
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

#endif // NET_TS_IP_UDP_HPP
