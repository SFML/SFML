//
// ip/address_v6.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_ADDRESS_V6_HPP
#define NET_TS_IP_ADDRESS_V6_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <string>
#include <experimental/__net_ts/detail/array.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>
#include <experimental/__net_ts/detail/string_view.hpp>
#include <experimental/__net_ts/detail/winsock_init.hpp>
#include <system_error>
#include <experimental/__net_ts/ip/address_v4.hpp>

#if !defined(NET_TS_NO_IOSTREAM)
# include <iosfwd>
#endif // !defined(NET_TS_NO_IOSTREAM)

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

template <typename> class basic_address_iterator;

/// Implements IP version 6 style addresses.
/**
 * The std::experimental::net::v1::ip::address_v6 class provides the ability to use and
 * manipulate IP version 6 addresses.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
class address_v6
{
public:
  /// The type used to represent an address as an array of bytes.
  /**
   * @note This type is defined in terms of the C++0x template @c std::array
   * when it is available. Otherwise, it uses @c boost:array.
   */
#if defined(GENERATING_DOCUMENTATION)
  typedef array<unsigned char, 16> bytes_type;
#else
  typedef std::experimental::net::v1::detail::array<unsigned char, 16> bytes_type;
#endif

  /// Default constructor.
  NET_TS_DECL address_v6();

  /// Construct an address from raw bytes and scope ID.
  NET_TS_DECL explicit address_v6(const bytes_type& bytes,
      unsigned long scope_id = 0);

  /// Copy constructor.
  NET_TS_DECL address_v6(const address_v6& other);

#if defined(NET_TS_HAS_MOVE)
  /// Move constructor.
  NET_TS_DECL address_v6(address_v6&& other);
#endif // defined(NET_TS_HAS_MOVE)

  /// Assign from another address.
  NET_TS_DECL address_v6& operator=(const address_v6& other);

#if defined(NET_TS_HAS_MOVE)
  /// Move-assign from another address.
  NET_TS_DECL address_v6& operator=(address_v6&& other);
#endif // defined(NET_TS_HAS_MOVE)

  /// The scope ID of the address.
  /**
   * Returns the scope ID associated with the IPv6 address.
   */
  unsigned long scope_id() const
  {
    return scope_id_;
  }

  /// The scope ID of the address.
  /**
   * Modifies the scope ID associated with the IPv6 address.
   */
  void scope_id(unsigned long id)
  {
    scope_id_ = id;
  }

  /// Get the address in bytes, in network byte order.
  NET_TS_DECL bytes_type to_bytes() const;

  /// Get the address as a string.
  NET_TS_DECL std::string to_string() const;

  /// Determine whether the address is a loopback address.
  NET_TS_DECL bool is_loopback() const;

  /// Determine whether the address is unspecified.
  NET_TS_DECL bool is_unspecified() const;

  /// Determine whether the address is link local.
  NET_TS_DECL bool is_link_local() const;

  /// Determine whether the address is site local.
  NET_TS_DECL bool is_site_local() const;

  /// Determine whether the address is a mapped IPv4 address.
  NET_TS_DECL bool is_v4_mapped() const;

  /// Determine whether the address is a multicast address.
  NET_TS_DECL bool is_multicast() const;

  /// Determine whether the address is a global multicast address.
  NET_TS_DECL bool is_multicast_global() const;

  /// Determine whether the address is a link-local multicast address.
  NET_TS_DECL bool is_multicast_link_local() const;

  /// Determine whether the address is a node-local multicast address.
  NET_TS_DECL bool is_multicast_node_local() const;

  /// Determine whether the address is a org-local multicast address.
  NET_TS_DECL bool is_multicast_org_local() const;

  /// Determine whether the address is a site-local multicast address.
  NET_TS_DECL bool is_multicast_site_local() const;

  /// Compare two addresses for equality.
  NET_TS_DECL friend bool operator==(
      const address_v6& a1, const address_v6& a2);

  /// Compare two addresses for inequality.
  friend bool operator!=(const address_v6& a1, const address_v6& a2)
  {
    return !(a1 == a2);
  }

  /// Compare addresses for ordering.
  NET_TS_DECL friend bool operator<(
      const address_v6& a1, const address_v6& a2);

  /// Compare addresses for ordering.
  friend bool operator>(const address_v6& a1, const address_v6& a2)
  {
    return a2 < a1;
  }

  /// Compare addresses for ordering.
  friend bool operator<=(const address_v6& a1, const address_v6& a2)
  {
    return !(a2 < a1);
  }

  /// Compare addresses for ordering.
  friend bool operator>=(const address_v6& a1, const address_v6& a2)
  {
    return !(a1 < a2);
  }

  /// Obtain an address object that represents any address.
  static address_v6 any()
  {
    return address_v6();
  }

  /// Obtain an address object that represents the loopback address.
  NET_TS_DECL static address_v6 loopback();

private:
  friend class basic_address_iterator<address_v6>;

  // The underlying IPv6 address.
  std::experimental::net::v1::detail::in6_addr_type addr_;

  // The scope ID associated with the address.
  unsigned long scope_id_;
};

/// Create an IPv6 address from raw bytes and scope ID.
/**
 * @relates address_v6
 */
inline address_v6 make_address_v6(const address_v6::bytes_type& bytes,
    unsigned long scope_id = 0)
{
  return address_v6(bytes, scope_id);
}

/// Create an IPv6 address from an IP address string.
/**
 * @relates address_v6
 */
NET_TS_DECL address_v6 make_address_v6(const char* str);

/// Create an IPv6 address from an IP address string.
/**
 * @relates address_v6
 */
NET_TS_DECL address_v6 make_address_v6(
    const char* str, std::error_code& ec);

/// Createan IPv6 address from an IP address string.
/**
 * @relates address_v6
 */
NET_TS_DECL address_v6 make_address_v6(const std::string& str);

/// Create an IPv6 address from an IP address string.
/**
 * @relates address_v6
 */
NET_TS_DECL address_v6 make_address_v6(
    const std::string& str, std::error_code& ec);

#if defined(NET_TS_HAS_STD_STRING_VIEW) \
  || defined(GENERATING_DOCUMENTATION)

/// Create an IPv6 address from an IP address string.
/**
 * @relates address_v6
 */
NET_TS_DECL address_v6 make_address_v6(string_view str);

/// Create an IPv6 address from an IP address string.
/**
 * @relates address_v6
 */
NET_TS_DECL address_v6 make_address_v6(
    string_view str, std::error_code& ec);

#endif // defined(NET_TS_HAS_STD_STRING_VIEW)
       //  || defined(GENERATING_DOCUMENTATION)

/// Tag type used for distinguishing overloads that deal in IPv4-mapped IPv6
/// addresses.
enum v4_mapped_t { v4_mapped };

/// Create an IPv4 address from a IPv4-mapped IPv6 address.
/**
 * @relates address_v4
 */
NET_TS_DECL address_v4 make_address_v4(
    v4_mapped_t, const address_v6& v6_addr);

/// Create an IPv4-mapped IPv6 address from an IPv4 address.
/**
 * @relates address_v6
 */
NET_TS_DECL address_v6 make_address_v6(
    v4_mapped_t, const address_v4& v4_addr);

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
 * @relates std::experimental::net::v1::ip::address_v6
 */
template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address_v6& addr);

#endif // !defined(NET_TS_NO_IOSTREAM)

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#include <experimental/__net_ts/ip/impl/address_v6.hpp>
#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/ip/impl/address_v6.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_IP_ADDRESS_V6_HPP
