//
// ip/unicast.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_UNICAST_HPP
#define NET_TS_IP_UNICAST_HPP

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
namespace unicast {

/// Socket option for time-to-live associated with outgoing unicast packets.
/**
 * Implements the IPPROTO_IP/IP_UNICAST_TTL socket option.
 *
 * @par Examples
 * Setting the option:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::unicast::hops option(4);
 * socket.set_option(option);
 * @endcode
 *
 * @par
 * Getting the current option value:
 * @code
 * std::experimental::net::ip::udp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::unicast::hops option;
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
typedef std::experimental::net::v1::ip::detail::socket_option::unicast_hops<
  NET_TS_OS_DEF(IPPROTO_IP),
  NET_TS_OS_DEF(IP_TTL),
  NET_TS_OS_DEF(IPPROTO_IPV6),
  NET_TS_OS_DEF(IPV6_UNICAST_HOPS)> hops;
#endif

} // namespace unicast
} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IP_UNICAST_HPP
