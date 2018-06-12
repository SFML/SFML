//
// ip/v6_only.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_V6_ONLY_HPP
#define NET_TS_IP_V6_ONLY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/socket_option.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

/// Socket option for determining whether an IPv6 socket supports IPv6
/// communication only.
/**
 * Implements the IPPROTO_IPV6/IP_V6ONLY socket option.
 *
 * @par Examples
 * Setting the option:
 * @code
 * std::experimental::net::ip::tcp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::v6_only option(true);
 * socket.set_option(option);
 * @endcode
 *
 * @par
 * Getting the current option value:
 * @code
 * std::experimental::net::ip::tcp::socket socket(io_context); 
 * ...
 * std::experimental::net::ip::v6_only option;
 * socket.get_option(option);
 * bool v6_only = option.value();
 * @endcode
 *
 * @par Concepts:
 * GettableSocketOption, SettableSocketOption.
 */
#if defined(GENERATING_DOCUMENTATION)
typedef implementation_defined v6_only;
#elif defined(IPV6_V6ONLY)
typedef std::experimental::net::v1::detail::socket_option::boolean<
    IPPROTO_IPV6, IPV6_V6ONLY> v6_only;
#else
typedef std::experimental::net::v1::detail::socket_option::boolean<
    std::experimental::net::v1::detail::custom_socket_option_level,
    std::experimental::net::v1::detail::always_fail_option> v6_only;
#endif

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IP_V6_ONLY_HPP
