//
// ip/address_v6_iterator.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//                         Oliver Kowalke (oliver dot kowalke at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_ADDRESS_V6_ITERATOR_HPP
#define NET_TS_IP_ADDRESS_V6_ITERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/ip/address_v6.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

template <typename> class basic_address_iterator;

/// An input iterator that can be used for traversing IPv6 addresses.
/**
 * In addition to satisfying the input iterator requirements, this iterator
 * also supports decrement.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
template <> class basic_address_iterator<address_v6>
{
public:
  /// The type of the elements pointed to by the iterator.
  typedef address_v6 value_type;

  /// Distance between two iterators.
  typedef std::ptrdiff_t difference_type;

  /// The type of a pointer to an element pointed to by the iterator.
  typedef const address_v6* pointer;

  /// The type of a reference to an element pointed to by the iterator.
  typedef const address_v6& reference;

  /// Denotes that the iterator satisfies the input iterator requirements.
  typedef std::input_iterator_tag iterator_category;

  /// Construct an iterator that points to the specified address.
  basic_address_iterator(const address_v6& addr) NET_TS_NOEXCEPT
    : address_(addr)
  {
  }

  /// Copy constructor.
  basic_address_iterator(
      const basic_address_iterator& other) NET_TS_NOEXCEPT
    : address_(other.address_)
  {
  }

#if defined(NET_TS_HAS_MOVE)
  /// Move constructor.
  basic_address_iterator(basic_address_iterator&& other) NET_TS_NOEXCEPT
    : address_(NET_TS_MOVE_CAST(address_v6)(other.address_))
  {
  }
#endif // defined(NET_TS_HAS_MOVE)

  /// Assignment operator.
  basic_address_iterator& operator=(
      const basic_address_iterator& other) NET_TS_NOEXCEPT
  {
    address_ = other.address_;
    return *this;
  }

#if defined(NET_TS_HAS_MOVE)
  /// Move assignment operator.
  basic_address_iterator& operator=(
      basic_address_iterator&& other) NET_TS_NOEXCEPT
  {
    address_ = NET_TS_MOVE_CAST(address_v6)(other.address_);
    return *this;
  }
#endif // defined(NET_TS_HAS_MOVE)

  /// Dereference the iterator.
  const address_v6& operator*() const NET_TS_NOEXCEPT
  {
    return address_;
  }

  /// Dereference the iterator.
  const address_v6* operator->() const NET_TS_NOEXCEPT
  {
    return &address_;
  }

  /// Pre-increment operator.
  basic_address_iterator& operator++() NET_TS_NOEXCEPT
  {
    for (int i = 15; i >= 0; --i)
    {
      if (address_.addr_.s6_addr[i] < 0xFF)
      {
        ++address_.addr_.s6_addr[i];
        break;
      }

      address_.addr_.s6_addr[i] = 0;
    }

    return *this;
  }

  /// Post-increment operator.
  basic_address_iterator operator++(int) NET_TS_NOEXCEPT
  {
    basic_address_iterator tmp(*this);
    ++*this;
    return tmp;
  }

  /// Pre-decrement operator.
  basic_address_iterator& operator--() NET_TS_NOEXCEPT
  {
    for (int i = 15; i >= 0; --i)
    {
      if (address_.addr_.s6_addr[i] > 0)
      {
        --address_.addr_.s6_addr[i];
        break;
      }

      address_.addr_.s6_addr[i] = 0xFF;
    }

    return *this;
  }

  /// Post-decrement operator.
  basic_address_iterator operator--(int)
  {
    basic_address_iterator tmp(*this);
    --*this;
    return tmp;
  }

  /// Compare two addresses for equality.
  friend bool operator==(const basic_address_iterator& a,
      const basic_address_iterator& b)
  {
    return a.address_ == b.address_;
  }

  /// Compare two addresses for inequality.
  friend bool operator!=(const basic_address_iterator& a,
      const basic_address_iterator& b)
  {
    return a.address_ != b.address_;
  }

private:
  address_v6 address_;
};

/// An input iterator that can be used for traversing IPv6 addresses.
typedef basic_address_iterator<address_v6> address_v6_iterator;

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IP_ADDRESS_V6_ITERATOR_HPP
