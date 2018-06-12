//
// ip/impl/address.ipp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_IMPL_ADDRESS_IPP
#define NET_TS_IP_IMPL_ADDRESS_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <typeinfo>
#include <experimental/__net_ts/detail/throw_error.hpp>
#include <experimental/__net_ts/detail/throw_exception.hpp>
#include <experimental/__net_ts/error.hpp>
#include <experimental/__net_ts/ip/address.hpp>
#include <experimental/__net_ts/ip/bad_address_cast.hpp>
#include <system_error>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

address::address()
  : type_(ipv4),
    ipv4_address_(),
    ipv6_address_()
{
}

address::address(const std::experimental::net::v1::ip::address_v4& ipv4_address)
  : type_(ipv4),
    ipv4_address_(ipv4_address),
    ipv6_address_()
{
}

address::address(const std::experimental::net::v1::ip::address_v6& ipv6_address)
  : type_(ipv6),
    ipv4_address_(),
    ipv6_address_(ipv6_address)
{
}

address::address(const address& other)
  : type_(other.type_),
    ipv4_address_(other.ipv4_address_),
    ipv6_address_(other.ipv6_address_)
{
}

#if defined(NET_TS_HAS_MOVE)
address::address(address&& other)
  : type_(other.type_),
    ipv4_address_(other.ipv4_address_),
    ipv6_address_(other.ipv6_address_)
{
}
#endif // defined(NET_TS_HAS_MOVE)

address& address::operator=(const address& other)
{
  type_ = other.type_;
  ipv4_address_ = other.ipv4_address_;
  ipv6_address_ = other.ipv6_address_;
  return *this;
}

#if defined(NET_TS_HAS_MOVE)
address& address::operator=(address&& other)
{
  type_ = other.type_;
  ipv4_address_ = other.ipv4_address_;
  ipv6_address_ = other.ipv6_address_;
  return *this;
}
#endif // defined(NET_TS_HAS_MOVE)

address& address::operator=(const std::experimental::net::v1::ip::address_v4& ipv4_address)
{
  type_ = ipv4;
  ipv4_address_ = ipv4_address;
  ipv6_address_ = std::experimental::net::v1::ip::address_v6();
  return *this;
}

address& address::operator=(const std::experimental::net::v1::ip::address_v6& ipv6_address)
{
  type_ = ipv6;
  ipv4_address_ = std::experimental::net::v1::ip::address_v4();
  ipv6_address_ = ipv6_address;
  return *this;
}

address make_address(const char* str)
{
  std::error_code ec;
  address addr = make_address(str, ec);
  std::experimental::net::v1::detail::throw_error(ec);
  return addr;
}

address make_address(const char* str, std::error_code& ec)
{
  std::experimental::net::v1::ip::address_v6 ipv6_address =
    std::experimental::net::v1::ip::make_address_v6(str, ec);
  if (!ec)
    return address(ipv6_address);

  std::experimental::net::v1::ip::address_v4 ipv4_address =
    std::experimental::net::v1::ip::make_address_v4(str, ec);
  if (!ec)
    return address(ipv4_address);

  return address();
}

address make_address(const std::string& str)
{
  return make_address(str.c_str());
}

address make_address(const std::string& str,
    std::error_code& ec)
{
  return make_address(str.c_str(), ec);
}

#if defined(NET_TS_HAS_STD_STRING_VIEW)

address make_address(string_view str)
{
  return make_address(static_cast<std::string>(str));
}

address make_address(string_view str,
    std::error_code& ec)
{
  return make_address(static_cast<std::string>(str), ec);
}

#endif // defined(NET_TS_HAS_STD_STRING_VIEW)

std::experimental::net::v1::ip::address_v4 address::to_v4() const
{
  if (type_ != ipv4)
  {
    bad_address_cast ex;
    std::experimental::net::v1::detail::throw_exception(ex);
  }
  return ipv4_address_;
}

std::experimental::net::v1::ip::address_v6 address::to_v6() const
{
  if (type_ != ipv6)
  {
    bad_address_cast ex;
    std::experimental::net::v1::detail::throw_exception(ex);
  }
  return ipv6_address_;
}

std::string address::to_string() const
{
  if (type_ == ipv6)
    return ipv6_address_.to_string();
  return ipv4_address_.to_string();
}

bool address::is_loopback() const
{
  return (type_ == ipv4)
    ? ipv4_address_.is_loopback()
    : ipv6_address_.is_loopback();
}

bool address::is_unspecified() const
{
  return (type_ == ipv4)
    ? ipv4_address_.is_unspecified()
    : ipv6_address_.is_unspecified();
}

bool address::is_multicast() const
{
  return (type_ == ipv4)
    ? ipv4_address_.is_multicast()
    : ipv6_address_.is_multicast();
}

bool operator==(const address& a1, const address& a2)
{
  if (a1.type_ != a2.type_)
    return false;
  if (a1.type_ == address::ipv6)
    return a1.ipv6_address_ == a2.ipv6_address_;
  return a1.ipv4_address_ == a2.ipv4_address_;
}

bool operator<(const address& a1, const address& a2)
{
  if (a1.type_ < a2.type_)
    return true;
  if (a1.type_ > a2.type_)
    return false;
  if (a1.type_ == address::ipv6)
    return a1.ipv6_address_ < a2.ipv6_address_;
  return a1.ipv4_address_ < a2.ipv4_address_;
}

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IP_IMPL_ADDRESS_IPP
