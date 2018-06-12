//
// detail/gcc_sync_fenced_block.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_GCC_SYNC_FENCED_BLOCK_HPP
#define NET_TS_DETAIL_GCC_SYNC_FENCED_BLOCK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(__GNUC__) \
  && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)) \
  && !defined(__INTEL_COMPILER) && !defined(__ICL) \
  && !defined(__ICC) && !defined(__ECC) && !defined(__PATHSCALE__)

#include <experimental/__net_ts/detail/noncopyable.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class gcc_sync_fenced_block
  : private noncopyable
{
public:
  enum half_or_full_t { half, full };

  // Constructor.
  explicit gcc_sync_fenced_block(half_or_full_t)
    : value_(0)
  {
    __sync_lock_test_and_set(&value_, 1);
  }

  // Destructor.
  ~gcc_sync_fenced_block()
  {
    __sync_lock_release(&value_);
  }

private:
  int value_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(__GNUC__)
       // && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4))
       // && !defined(__INTEL_COMPILER) && !defined(__ICL)
       // && !defined(__ICC) && !defined(__ECC) && !defined(__PATHSCALE__)

#endif // NET_TS_DETAIL_GCC_SYNC_FENCED_BLOCK_HPP
