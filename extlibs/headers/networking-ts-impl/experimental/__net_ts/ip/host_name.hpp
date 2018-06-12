//
// ip/host_name.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_HOST_NAME_HPP
#define NET_TS_IP_HOST_NAME_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <string>
#include <system_error>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

/// Get the current host name.
NET_TS_DECL std::string host_name();

/// Get the current host name.
NET_TS_DECL std::string host_name(std::error_code& ec);

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/ip/impl/host_name.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_IP_HOST_NAME_HPP
