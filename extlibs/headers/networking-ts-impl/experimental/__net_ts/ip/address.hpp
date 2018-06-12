//
// ip/address.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_ADDRESS_HPP
#define NET_TS_IP_ADDRESS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <string>
#include <experimental/__net_ts/detail/throw_exception.hpp>
#include <experimental/__net_ts/detail/string_view.hpp>
#include <experimental/__net_ts/detail/type_traits.hpp>
#include <system_error>
#include <experimental/__net_ts/ip/address_v4.hpp>
#include <experimental/__net_ts/ip/address_v6.hpp>
#include <experimental/__net_ts/ip/bad_address_cast.hpp>

#if !defined(NET_TS_NO_IOSTREAM)
# include <iosfwd>
#endif // !defined(NET_TS_NO_IOSTREAM)

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

/// Implements version-independent IP addresses.
/**
 * The std::experimental::net::v1::ip::address class provides the ability to use either IP
 * version 4 or version 6 addresses.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
class address
{
public:
  /// Default constructor.
  NET_TS_DECL address();

  /// Construct an address from an IPv4 address.
  NET_TS_DECL address(const std::experimental::net::v1::ip::address_v4& ipv4_address);

  /// Construct an address from an IPv6 address.
  NET_TS_DECL address(const std::experimental::net::v1::ip::address_v6& ipv6_address);

  /// Copy constructor.
  NET_TS_DECL address(const address& other);

#if defined(NET_TS_HAS_MOVE)
  /// Move constructor.
  NET_TS_DECL address(address&& other);
#endif // defined(NET_TS_HAS_MOVE)

  /// Assign from another address.
  NET_TS_DECL address& operator=(const address& other);

#if defined(NET_TS_HAS_MOVE)
  /// Move-assign from another address.
  NET_TS_DECL address& operator=(address&& other);
#endif // defined(NET_TS_HAS_MOVE)

  /// Assign from an IPv4 address.
  NET_TS_DECL address& operator=(
      const std::experimental::net::v1::ip::address_v4& ipv4_address);

  /// Assign from an IPv6 address.
  NET_TS_DECL address& operator=(
      const std::experimental::net::v1::ip::address_v6& ipv6_address);

  /// Get whether the address is an IP version 4 address.
  bool is_v4() const
  {
    return type_ == ipv4;
  }

  /// Get whether the address is an IP version 6 address.
  bool is_v6() const
  {
    return type_ == ipv6;
  }

  /// Get the address as an IP version 4 address.
  NET_TS_DECL std::experimental::net::v1::ip::address_v4 to_v4() const;

  /// Get the address as an IP version 6 address.
  NET_TS_DECL std::experimental::net::v1::ip::address_v6 to_v6() const;

  /// Get the address as a string.
  NET_TS_DECL std::string to_string() const;

  /// Determine whether the address is a loopback address.
  NET_TS_DECL bool is_loopback() const;

  /// Determine whether the address is unspecified.
  NET_TS_DECL bool is_unspecified() const;

  /// Determine whether the address is a multicast address.
  NET_TS_DECL bool is_multicast() const;

  /// Compare two addresses for equality.
  NET_TS_DECL friend bool operator==(const address& a1, const address& a2);

  /// Compare two addresses for inequality.
  friend bool operator!=(const address& a1, const address& a2)
  {
    return !(a1 == a2);
  }

  /// Compare addresses for ordering.
  NET_TS_DECL friend bool operator<(const address& a1, const address& a2);

  /// Compare addresses for ordering.
  friend bool operator>(const address& a1, const address& a2)
  {
    return a2 < a1;
  }

  /// Compare addresses for ordering.
  friend bool operator<=(const address& a1, const address& a2)
  {
    return !(a2 < a1);
  }

  /// Compare addresses for ordering.
  friend bool operator>=(const address& a1, const address& a2)
  {
    return !(a1 < a2);
  }

private:
  // The type of the address.
  enum { ipv4, ipv6 } type_;

  // The underlying IPv4 address.
  std::experimental::net::v1::ip::address_v4 ipv4_address_;

  // The underlying IPv6 address.
  std::experimental::net::v1::ip::address_v6 ipv6_address_;
};

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
/**
 * @relates address
 */
NET_TS_DECL address make_address(const char* str);

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
/**
 * @relates address
 */
NET_TS_DECL address make_address(
    const char* str, std::error_code& ec);

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
/**
 * @relates address
 */
NET_TS_DECL address make_address(const std::string& str);

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
/**
 * @relates address
 */
NET_TS_DECL address make_address(
    const std::string& str, std::error_code& ec);

#if defined(NET_TS_HAS_STD_STRING_VIEW) \
  || defined(GENERATING_DOCUMENTATION)

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
/**
 * @relates address
 */
NET_TS_DECL address make_address(string_view str);

/// Create an address from an IPv4 address string in dotted decimal form,
/// or from an IPv6 address in hexadecimal notation.
/**
 * @relates address
 */
NET_TS_DECL address make_address(
    string_view str, std::error_code& ec);

#endif // defined(NET_TS_HAS_STD_STRING_VIEW)
       //  || defined(GENERATING_DOCUMENTATION)

#if !defined(NET_TS_NO_IOSTREAM)

/// Output an address as a string.
/**
 * Used to output a human-readable string for a specified address.
 *
 * @param os The output stream to which the string will be written.
 *
 * @param addr The address to be written.
 *
 * @return The output stream.
 *
 * @relates std::experimental::net::v1::ip::address
 */
template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address& addr);

#endif // !defined(NET_TS_NO_IOSTREAM)

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#include <experimental/__net_ts/ip/impl/address.hpp>
#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/ip/impl/address.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_IP_ADDRESS_HPP
