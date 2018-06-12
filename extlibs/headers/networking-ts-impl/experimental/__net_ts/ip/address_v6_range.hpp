//
// ip/address_v6_range.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//                         Oliver Kowalke (oliver dot kowalke at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IP_ADDRESS_V6_RANGE_HPP
#define NET_TS_IP_ADDRESS_V6_RANGE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/ip/address_v6_iterator.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace ip {

template <typename> class basic_address_range;

/// Represents a range of IPv6 addresses.
/**
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
template <> class basic_address_range<address_v6>
{
public:
  /// The type of an iterator that points into the range.
  typedef basic_address_iterator<address_v6> iterator;

  /// Construct an empty range.
  basic_address_range() NET_TS_NOEXCEPT
    : begin_(address_v6()),
      end_(address_v6())
  {
  }

  /// Construct an range that represents the given range of addresses.
  explicit basic_address_range(const iterator& first,
      const iterator& last) NET_TS_NOEXCEPT
    : begin_(first),
      end_(last)
  {
  }

  /// Copy constructor.
  basic_address_range(const basic_address_range& other) NET_TS_NOEXCEPT
    : begin_(other.begin_),
      end_(other.end_)
  {
  }

#if defined(NET_TS_HAS_MOVE)
  /// Move constructor.
  basic_address_range(basic_address_range&& other) NET_TS_NOEXCEPT
    : begin_(NET_TS_MOVE_CAST(iterator)(other.begin_)),
      end_(NET_TS_MOVE_CAST(iterator)(other.end_))
  {
  }
#endif // defined(NET_TS_HAS_MOVE)

  /// Assignment operator.
  basic_address_range& operator=(
      const basic_address_range& other) NET_TS_NOEXCEPT
  {
    begin_ = other.begin_;
    end_ = other.end_;
    return *this;
  }

#if defined(NET_TS_HAS_MOVE)
  /// Move assignment operator.
  basic_address_range& operator=(
      basic_address_range&& other) NET_TS_NOEXCEPT
  {
    begin_ = NET_TS_MOVE_CAST(iterator)(other.begin_);
    end_ = NET_TS_MOVE_CAST(iterator)(other.end_);
    return *this;
  }
#endif // defined(NET_TS_HAS_MOVE)

  /// Obtain an iterator that points to the start of the range.
  iterator begin() const NET_TS_NOEXCEPT
  {
    return begin_;
  }

  /// Obtain an iterator that points to the end of the range.
  iterator end() const NET_TS_NOEXCEPT
  {
    return end_;
  }

  /// Determine whether the range is empty.
  bool empty() const NET_TS_NOEXCEPT
  {
    return begin_ == end_;
  }

  /// Find an address in the range.
  iterator find(const address_v6& addr) const NET_TS_NOEXCEPT
  {
    return addr >= *begin_ && addr < *end_ ? iterator(addr) : end_;
  }

private:
  iterator begin_;
  iterator end_;
};

/// Represents a range of IPv6 addresses.
typedef basic_address_range<address_v6> address_v6_range;

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IP_ADDRESS_V6_RANGE_HPP
