//
// ip/address_v4.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_ADDRESS_V4_HPP
#define NET_TS_IP_ADDRESS_V4_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <string>
#include <experimental/__net_ts/detail/array.hpp>
#include <experimental/__net_ts/detail/cstdint.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>
#include <experimental/__net_ts/detail/string_view.hpp>
#include <experimental/__net_ts/detail/winsock_init.hpp>
#include <system_error>

#if !defined(NET_TS_NO_IOSTREAM)
# include <iosfwd>
#endif // !defined(NET_TS_NO_IOSTREAM)

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

/// Implements IP version 4 style addresses.
/**
 * The std::experimental::net::v1::ip::address_v4 class provides the ability to use and
 * manipulate IP version 4 addresses.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
class address_v4
{
public:
  /// The type used to represent an address as an unsigned integer.
  typedef uint_least32_t uint_type;

  /// The type used to represent an address as an array of bytes.
  /**
   * @note This type is defined in terms of the C++0x template @c std::array
   * when it is available. Otherwise, it uses @c boost:array.
   */
#if defined(GENERATING_DOCUMENTATION)
  typedef array<unsigned char, 4> bytes_type;
#else
  typedef std::experimental::net::v1::detail::array<unsigned char, 4> bytes_type;
#endif

  /// Default constructor.
  address_v4()
  {
    addr_.s_addr = 0;
  }

  /// Construct an address from raw bytes.
  NET_TS_DECL explicit address_v4(const bytes_type& bytes);

  /// Construct an address from an unsigned integer in host byte order.
  NET_TS_DECL explicit address_v4(uint_type addr);

  /// Copy constructor.
  address_v4(const address_v4& other)
    : addr_(other.addr_)
  {
  }

#if defined(NET_TS_HAS_MOVE)
  /// Move constructor.
  address_v4(address_v4&& other)
    : addr_(other.addr_)
  {
  }
#endif // defined(NET_TS_HAS_MOVE)

  /// Assign from another address.
  address_v4& operator=(const address_v4& other)
  {
    addr_ = other.addr_;
    return *this;
  }

#if defined(NET_TS_HAS_MOVE)
  /// Move-assign from another address.
  address_v4& operator=(address_v4&& other)
  {
    addr_ = other.addr_;
    return *this;
  }
#endif // defined(NET_TS_HAS_MOVE)

  /// Get the address in bytes, in network byte order.
  NET_TS_DECL bytes_type to_bytes() const;

  /// Get the address as an unsigned integer in host byte order
  NET_TS_DECL uint_type to_uint() const;

  /// Get the address as a string in dotted decimal format.
  NET_TS_DECL std::string to_string() const;

  /// Determine whether the address is a loopback address.
  NET_TS_DECL bool is_loopback() const;

  /// Determine whether the address is unspecified.
  NET_TS_DECL bool is_unspecified() const;

  /// Determine whether the address is a multicast address.
  NET_TS_DECL bool is_multicast() const;

  /// Compare two addresses for equality.
  friend bool operator==(const address_v4& a1, const address_v4& a2)
  {
    return a1.addr_.s_addr == a2.addr_.s_addr;
  }

  /// Compare two addresses for inequality.
  friend bool operator!=(const address_v4& a1, const address_v4& a2)
  {
    return a1.addr_.s_addr != a2.addr_.s_addr;
  }

  /// Compare addresses for ordering.
  friend bool operator<(const address_v4& a1, const address_v4& a2)
  {
    return a1.to_uint() < a2.to_uint();
  }

  /// Compare addresses for ordering.
  friend bool operator>(const address_v4& a1, const address_v4& a2)
  {
    return a1.to_uint() > a2.to_uint();
  }

  /// Compare addresses for ordering.
  friend bool operator<=(const address_v4& a1, const address_v4& a2)
  {
    return a1.to_uint() <= a2.to_uint();
  }

  /// Compare addresses for ordering.
  friend bool operator>=(const address_v4& a1, const address_v4& a2)
  {
    return a1.to_uint() >= a2.to_uint();
  }

  /// Obtain an address object that represents any address.
  static address_v4 any()
  {
    return address_v4();
  }

  /// Obtain an address object that represents the loopback address.
  static address_v4 loopback()
  {
    return address_v4(0x7F000001);
  }

  /// Obtain an address object that represents the broadcast address.
  static address_v4 broadcast()
  {
    return address_v4(0xFFFFFFFF);
  }

private:
  // The underlying IPv4 address.
  std::experimental::net::v1::detail::in4_addr_type addr_;
};

/// Create an IPv4 address from raw bytes in network order.
/**
 * @relates address_v4
 */
inline address_v4 make_address_v4(const address_v4::bytes_type& bytes)
{
  return address_v4(bytes);
}

/// Create an IPv4 address from an unsigned integer in host byte order.
/**
 * @relates address_v4
 */
inline address_v4 make_address_v4(address_v4::uint_type addr)
{
  return address_v4(addr);
}

/// Create an IPv4 address from an IP address string in dotted decimal form.
/**
 * @relates address_v4
 */
NET_TS_DECL address_v4 make_address_v4(const char* str);

/// Create an IPv4 address from an IP address string in dotted decimal form.
/**
 * @relates address_v4
 */
NET_TS_DECL address_v4 make_address_v4(
    const char* str, std::error_code& ec);

/// Create an IPv4 address from an IP address string in dotted decimal form.
/**
 * @relates address_v4
 */
NET_TS_DECL address_v4 make_address_v4(const std::string& str);

/// Create an IPv4 address from an IP address string in dotted decimal form.
/**
 * @relates address_v4
 */
NET_TS_DECL address_v4 make_address_v4(
    const std::string& str, std::error_code& ec);

#if defined(NET_TS_HAS_STD_STRING_VIEW) \
  || defined(GENERATING_DOCUMENTATION)

/// Create an IPv4 address from an IP address string in dotted decimal form.
/**
 * @relates address_v4
 */
NET_TS_DECL address_v4 make_address_v4(string_view str);

/// Create an IPv4 address from an IP address string in dotted decimal form.
/**
 * @relates address_v4
 */
NET_TS_DECL address_v4 make_address_v4(
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
 * @relates std::experimental::net::v1::ip::address_v4
 */
template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address_v4& addr);

#endif // !defined(NET_TS_NO_IOSTREAM)

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#include <experimental/__net_ts/ip/impl/address_v4.hpp>
#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/ip/impl/address_v4.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_IP_ADDRESS_V4_HPP
