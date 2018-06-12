//
// ip/multicast.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_MULTICAST_HPP
#define NET_TS_IP_MULTICAST_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <cstddef>
#include <experimental/__net_ts/ip/detail/socket_option.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {
namespace multicast {

/// Socket option to join a multicast group on a specified interface.
/**
 * Implements the IPPROTO_IP/IP_ADD_MEMBERSHIP socket option.
 *
 * @par Examples
 * Setting the option to join a multicast group:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::address multicast_address =
 *   std::experimental::net::ip::address::from_string("225.0.0.1");
 * std::experimental::net::ip::multicast::join_group option(multicast_address);
 * socket.set_option(option);
 * @endcode
 *
 * @par Concepts:
 * SettableSocketOption.
 */
#if defined(GENERATING_DOCUMENTATION)
typedef implementation_defined join_group;
#else
typedef std::experimental::net::v1::ip::detail::socket_option::multicast_request<
  NET_TS_OS_DEF(IPPROTO_IP),
  NET_TS_OS_DEF(IP_ADD_MEMBERSHIP),
  NET_TS_OS_DEF(IPPROTO_IPV6),
  NET_TS_OS_DEF(IPV6_JOIN_GROUP)> join_group;
#endif

/// Socket option to leave a multicast group on a specified interface.
/**
 * Implements the IPPROTO_IP/IP_DROP_MEMBERSHIP socket option.
 *
 * @par Examples
 * Setting the option to leave a multicast group:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::address multicast_address =
 *   std::experimental::net::ip::address::from_string("225.0.0.1");
 * std::experimental::net::ip::multicast::leave_group option(multicast_address);
 * socket.set_option(option);
 * @endcode
 *
 * @par Concepts:
 * SettableSocketOption.
 */
#if defined(GENERATING_DOCUMENTATION)
typedef implementation_defined leave_group;
#else
typedef std::experimental::net::v1::ip::detail::socket_option::multicast_request<
  NET_TS_OS_DEF(IPPROTO_IP),
  NET_TS_OS_DEF(IP_DROP_MEMBERSHIP),
  NET_TS_OS_DEF(IPPROTO_IPV6),
  NET_TS_OS_DEF(IPV6_LEAVE_GROUP)> leave_group;
#endif

/// Socket option for local interface to use for outgoing multicast packets.
/**
 * Implements the IPPROTO_IP/IP_MULTICAST_IF socket option.
 *
 * @par Examples
 * Setting the option:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::address_v4 local_interface =
 *   std::experimental::net::ip::address_v4::from_string("1.2.3.4");
 * std::experimental::net::ip::multicast::outbound_interface option(local_interface);
 * socket.set_option(option);
 * @endcode
 *
 * @par Concepts:
 * SettableSocketOption.
 */
#if defined(GENERATING_DOCUMENTATION)
typedef implementation_defined outbound_interface;
#else
typedef std::experimental::net::v1::ip::detail::socket_option::network_interface<
  NET_TS_OS_DEF(IPPROTO_IP),
  NET_TS_OS_DEF(IP_MULTICAST_IF),
  NET_TS_OS_DEF(IPPROTO_IPV6),
  NET_TS_OS_DEF(IPV6_MULTICAST_IF)> outbound_interface;
#endif

/// Socket option for time-to-live associated with outgoing multicast packets.
/**
 * Implements the IPPROTO_IP/IP_MULTICAST_TTL socket option.
 *
 * @par Examples
 * Setting the option:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::multicast::hops option(4);
 * socket.set_option(option);
 * @endcode
 *
 * @par
 * Getting the current option value:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::multicast::hops option;
 * socket.get_option(option);
 * int ttl = option.value();
 * @endcode
 *
 * @par Concepts:
 * GettableSocketOption, SettableSocketOption.
 */
#if defined(GENERATING_DOCUMENTATION)
typedef implementation_defined hops;
#else
typedef std::experimental::net::v1::ip::detail::socket_option::multicast_hops<
  NET_TS_OS_DEF(IPPROTO_IP),
  NET_TS_OS_DEF(IP_MULTICAST_TTL),
  NET_TS_OS_DEF(IPPROTO_IPV6),
  NET_TS_OS_DEF(IPV6_MULTICAST_HOPS)> hops;
#endif

/// Socket option determining whether outgoing multicast packets will be
/// received on the same socket if it is a member of the multicast group.
/**
 * Implements the IPPROTO_IP/IP_MULTICAST_LOOP socket option.
 *
 * @par Examples
 * Setting the option:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::multicast::enable_loopback option(true);
 * socket.set_option(option);
 * @endcode
 *
 * @par
 * Getting the current option value:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::multicast::enable_loopback option;
 * socket.get_option(option);
 * bool is_set = option.value();
 * @endcode
 *
 * @par Concepts:
 * GettableSocketOption, SettableSocketOption.
 */
#if defined(GENERATING_DOCUMENTATION)
typedef implementation_defined enable_loopback;
#else
typedef std::experimental::net::v1::ip::detail::socket_option::multicast_enable_loopback<
  NET_TS_OS_DEF(IPPROTO_IP),
  NET_TS_OS_DEF(IP_MULTICAST_LOOP),
  NET_TS_OS_DEF(IPPROTO_IPV6),
  NET_TS_OS_DEF(IPV6_MULTICAST_LOOP)> enable_loopback;
#endif

} // namespace multicast
} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IP_MULTICAST_HPP
