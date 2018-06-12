//
// ip/network_v6.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2014 Oliver Kowalke (oliver dot kowalke at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_NETWORK_V6_HPP
#define NET_TS_IP_NETWORK_V6_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <string>
#include <experimental/__net_ts/detail/string_view.hpp>
#include <system_error>
#include <experimental/__net_ts/ip/address_v6_range.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

/// Represents an IPv6 network.
/**
 * The std::experimental::net::v1::ip::network_v6 class provides the ability to use and
 * manipulate IP version 6 networks.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
class network_v6
{
public:
  /// Default constructor.
  network_v6() NET_TS_NOEXCEPT
    : address_(),
      prefix_length_(0)
  {
  }

  /// Construct a network based on the specified address and prefix length.
  NET_TS_DECL network_v6(const address_v6& addr,
      unsigned short prefix_len);

  /// Copy constructor.
  network_v6(const network_v6& other) NET_TS_NOEXCEPT
    : address_(other.address_),
      prefix_length_(other.prefix_length_)
  {
  }

#if defined(NET_TS_HAS_MOVE)
  /// Move constructor.
  network_v6(network_v6&& other) NET_TS_NOEXCEPT
    : address_(NET_TS_MOVE_CAST(address_v6)(other.address_)),
      prefix_length_(other.prefix_length_)
  {
  }
#endif // defined(NET_TS_HAS_MOVE)

  /// Assign from another network.
  network_v6& operator=(const network_v6& other) NET_TS_NOEXCEPT
  {
    address_ = other.address_;
    prefix_length_ = other.prefix_length_;
    return *this;
  }

#if defined(NET_TS_HAS_MOVE)
  /// Move-assign from another network.
  network_v6& operator=(network_v6&& other) NET_TS_NOEXCEPT
  {
    address_ = NET_TS_MOVE_CAST(address_v6)(other.address_);
    prefix_length_ = other.prefix_length_;
    return *this;
  }
#endif // defined(NET_TS_HAS_MOVE)

  /// Obtain the address object specified when the network object was created.
  address_v6 address() const NET_TS_NOEXCEPT
  {
    return address_;
  }

  /// Obtain the prefix length that was specified when the network object was
  /// created.
  unsigned short prefix_length() const NET_TS_NOEXCEPT
  {
    return prefix_length_;
  }

  /// Obtain an address object that represents the network address.
  NET_TS_DECL address_v6 network() const NET_TS_NOEXCEPT;

  /// Obtain an address range corresponding to the hosts in the network.
  NET_TS_DECL address_v6_range hosts() const NET_TS_NOEXCEPT;

  /// Obtain the true network address, omitting any host bits.
  network_v6 canonical() const NET_TS_NOEXCEPT
  {
    return network_v6(network(), prefix_length());
  }

  /// Test if network is a valid host address.
  bool is_host() const NET_TS_NOEXCEPT
  {
    return prefix_length_ == 128;
  }

  /// Test if a network is a real subnet of another network.
  NET_TS_DECL bool is_subnet_of(const network_v6& other) const;

  /// Get the network as an address in dotted decimal format.
  NET_TS_DECL std::string to_string() const;

  /// Get the network as an address in dotted decimal format.
  NET_TS_DECL std::string to_string(std::error_code& ec) const;

  /// Compare two networks for equality.
  friend bool operator==(const network_v6& a, const network_v6& b)
  {
    return a.address_ == b.address_ && a.prefix_length_ == b.prefix_length_;
  }

  /// Compare two networks for inequality.
  friend bool operator!=(const network_v6& a, const network_v6& b)
  {
    return !(a == b);
  }

private:
  address_v6 address_;
  unsigned short prefix_length_;
};

/// Create an IPv6 network from an address and prefix length.
/**
 * @relates address_v6
 */
inline network_v6 make_network_v6(
    const address_v6& addr, unsigned short prefix_len)
{
  return network_v6(addr, prefix_len);
}

/// Create an IPv6 network from a string containing IP address and prefix
/// length.
/**
 * @relates network_v6
 */
NET_TS_DECL network_v6 make_network_v6(const char* str);

/// Create an IPv6 network from a string containing IP address and prefix
/// length.
/**
 * @relates network_v6
 */
NET_TS_DECL network_v6 make_network_v6(
    const char* str, std::error_code& ec);

/// Create an IPv6 network from a string containing IP address and prefix
/// length.
/**
 * @relates network_v6
 */
NET_TS_DECL network_v6 make_network_v6(const std::string& str);

/// Create an IPv6 network from a string containing IP address and prefix
/// length.
/**
 * @relates network_v6
 */
NET_TS_DECL network_v6 make_network_v6(
    const std::string& str, std::error_code& ec);

#if defined(NET_TS_HAS_STD_STRING_VIEW) \
  || defined(GENERATING_DOCUMENTATION)

/// Create an IPv6 network from a string containing IP address and prefix
/// length.
/**
 * @relates network_v6
 */
NET_TS_DECL network_v6 make_network_v6(string_view str);

/// Create an IPv6 network from a string containing IP address and prefix
/// length.
/**
 * @relates network_v6
 */
NET_TS_DECL network_v6 make_network_v6(
    string_view str, std::error_code& ec);

#endif // defined(NET_TS_HAS_STD_STRING_VIEW)
       //  || defined(GENERATING_DOCUMENTATION)

#if !defined(NET_TS_NO_IOSTREAM)

/// Output a network as a string.
/**
 * Used to output a human-readable string for a specified network.
 *
 * @param os The output stream to which the string will be written.
 *
 * @param net The network to be written.
 *
 * @return The output stream.
 *
 * @relates std::experimental::net::v1::ip::address_v6
 */
template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const network_v6& net);

#endif // !defined(NET_TS_NO_IOSTREAM)

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#include <experimental/__net_ts/ip/impl/network_v6.hpp>
#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/ip/impl/network_v6.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_IP_NETWORK_V6_HPP
