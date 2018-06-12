//
// ip/detail/endpoint.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_DETAIL_ENDPOINT_HPP
#define NET_TS_IP_DETAIL_ENDPOINT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <string>
#include <experimental/__net_ts/detail/socket_types.hpp>
#include <experimental/__net_ts/detail/winsock_init.hpp>
#include <system_error>
#include <experimental/__net_ts/ip/address.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {
namespace detail {

// Helper class for implementating an IP endpoint.
class endpoint
{
public:
  // Default constructor.
  NET_TS_DECL endpoint();

  // Construct an endpoint using a family and port number.
  NET_TS_DECL endpoint(int family, unsigned short port_num);

  // Construct an endpoint using an address and port number.
  NET_TS_DECL endpoint(const std::experimental::net::v1::ip::address& addr,
      unsigned short port_num);

  // Copy constructor.
  endpoint(const endpoint& other)
    : data_(other.data_)
  {
  }

  // Assign from another endpoint.
  endpoint& operator=(const endpoint& other)
  {
    data_ = other.data_;
    return *this;
  }

  // Get the underlying endpoint in the native type.
  std::experimental::net::v1::detail::socket_addr_type* data()
  {
    return &data_.base;
  }

  // Get the underlying endpoint in the native type.
  const std::experimental::net::v1::detail::socket_addr_type* data() const
  {
    return &data_.base;
  }

  // Get the underlying size of the endpoint in the native type.
  std::size_t size() const
  {
    if (is_v4())
      return sizeof(std::experimental::net::v1::detail::sockaddr_in4_type);
    else
      return sizeof(std::experimental::net::v1::detail::sockaddr_in6_type);
  }

  // Set the underlying size of the endpoint in the native type.
  NET_TS_DECL void resize(std::size_t new_size);

  // Get the capacity of the endpoint in the native type.
  std::size_t capacity() const
  {
    return sizeof(data_);
  }

  // Get the port associated with the endpoint.
  NET_TS_DECL unsigned short port() const;

  // Set the port associated with the endpoint.
  NET_TS_DECL void port(unsigned short port_num);

  // Get the IP address associated with the endpoint.
  NET_TS_DECL std::experimental::net::v1::ip::address address() const;

  // Set the IP address associated with the endpoint.
  NET_TS_DECL void address(const std::experimental::net::v1::ip::address& addr);

  // Compare two endpoints for equality.
  NET_TS_DECL friend bool operator==(
      const endpoint& e1, const endpoint& e2);

  // Compare endpoints for ordering.
  NET_TS_DECL friend bool operator<(
      const endpoint& e1, const endpoint& e2);

  // Determine whether the endpoint is IPv4.
  bool is_v4() const
  {
    return data_.base.sa_family == NET_TS_OS_DEF(AF_INET);
  }

#if !defined(NET_TS_NO_IOSTREAM)
  // Convert to a string.
  NET_TS_DECL std::string to_string() const;
#endif // !defined(NET_TS_NO_IOSTREAM)

private:
  // The underlying IP socket address.
  union data_union
  {
    std::experimental::net::v1::detail::socket_addr_type base;
    std::experimental::net::v1::detail::sockaddr_in4_type v4;
    std::experimental::net::v1::detail::sockaddr_in6_type v6;
  } data_;
};

} // namespace detail
} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/ip/detail/impl/endpoint.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_IP_DETAIL_ENDPOINT_HPP
