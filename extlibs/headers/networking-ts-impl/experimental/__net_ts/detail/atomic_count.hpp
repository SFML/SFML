//
// detail/atomic_count.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_ATOMIC_COUNT_HPP
#define NET_TS_DETAIL_ATOMIC_COUNT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_THREADS)
// Nothing to include.
#elif defined(NET_TS_HAS_STD_ATOMIC)
# include <atomic>
#else // defined(NET_TS_HAS_STD_ATOMIC)
# include <boost/detail/atomic_count.hpp>
#endif // defined(NET_TS_HAS_STD_ATOMIC)

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if !defined(NET_TS_HAS_THREADS)
typedef long atomic_count;
inline void increment(atomic_count& a, long b) { a += b; }
#elif defined(NET_TS_HAS_STD_ATOMIC)
typedef std::atomic<long> atomic_count;
inline void increment(atomic_count& a, long b) { a += b; }
#else // defined(NET_TS_HAS_STD_ATOMIC)
typedef boost::detail::atomic_count atomic_count;
inline void increment(atomic_count& a, long b) { while (b > 0) ++a, --b; }
#endif // defined(NET_TS_HAS_STD_ATOMIC)

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_ATOMIC_COUNT_HPP
