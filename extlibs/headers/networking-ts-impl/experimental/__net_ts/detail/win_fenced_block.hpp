//
// detail/win_fenced_block.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_WIN_FENCED_BLOCK_HPP
#define NET_TS_DETAIL_WIN_FENCED_BLOCK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS) && !defined(UNDER_CE)

#include <experimental/__net_ts/detail/socket_types.hpp>
#include <experimental/__net_ts/detail/noncopyable.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class win_fenced_block
  : private noncopyable
{
public:
  enum half_t { half };
  enum full_t { full };

  // Constructor for a half fenced block.
  explicit win_fenced_block(half_t)
  {
  }

  // Constructor for a full fenced block.
  explicit win_fenced_block(full_t)
  {
#if defined(__BORLANDC__)
    LONG barrier = 0;
    ::InterlockedExchange(&barrier, 1);
#elif defined(NET_TS_MSVC) \
  && ((NET_TS_MSVC < 1400) || !defined(MemoryBarrier))
# if defined(_M_IX86)
#  pragma warning(push)
#  pragma warning(disable:4793)
    LONG barrier;
    __asm { xchg barrier, eax }
#  pragma warning(pop)
# endif // defined(_M_IX86)
#else
    MemoryBarrier();
#endif
  }

  // Destructor.
  ~win_fenced_block()
  {
#if defined(__BORLANDC__)
    LONG barrier = 0;
    ::InterlockedExchange(&barrier, 1);
#elif defined(NET_TS_MSVC) \
  && ((NET_TS_MSVC < 1400) || !defined(MemoryBarrier))
# if defined(_M_IX86)
#  pragma warning(push)
#  pragma warning(disable:4793)
    LONG barrier;
    __asm { xchg barrier, eax }
#  pragma warning(pop)
# endif // defined(_M_IX86)
#else
    MemoryBarrier();
#endif
  }
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_WINDOWS) && !defined(UNDER_CE)

#endif // NET_TS_DETAIL_WIN_FENCED_BLOCK_HPP
