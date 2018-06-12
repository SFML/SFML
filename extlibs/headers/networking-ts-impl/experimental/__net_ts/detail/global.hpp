//
// detail/global.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_GLOBAL_HPP
#define NET_TS_DETAIL_GLOBAL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_THREADS)
# include <experimental/__net_ts/detail/null_global.hpp>
#elif defined(NET_TS_WINDOWS)
# include <experimental/__net_ts/detail/win_global.hpp>
#elif defined(NET_TS_HAS_PTHREADS)
# include <experimental/__net_ts/detail/posix_global.hpp>
#elif defined(NET_TS_HAS_STD_CALL_ONCE)
# include <experimental/__net_ts/detail/std_global.hpp>
#else
# error Only Windows, POSIX and std::call_once are supported!
#endif

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename T>
inline T& global()
{
#if !defined(NET_TS_HAS_THREADS)
  return null_global<T>();
#elif defined(NET_TS_WINDOWS)
  return win_global<T>();
#elif defined(NET_TS_HAS_PTHREADS)
  return posix_global<T>();
#elif defined(NET_TS_HAS_STD_CALL_ONCE)
  return std_global<T>();
#endif
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_GLOBAL_HPP
