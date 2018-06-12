//
// detail/static_mutex.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_STATIC_MUTEX_HPP
#define NET_TS_DETAIL_STATIC_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_THREADS)
# include <experimental/__net_ts/detail/null_static_mutex.hpp>
#elif defined(NET_TS_WINDOWS)
# include <experimental/__net_ts/detail/win_static_mutex.hpp>
#elif defined(NET_TS_HAS_PTHREADS)
# include <experimental/__net_ts/detail/posix_static_mutex.hpp>
#elif defined(NET_TS_HAS_STD_MUTEX_AND_CONDVAR)
# include <experimental/__net_ts/detail/std_static_mutex.hpp>
#else
# error Only Windows and POSIX are supported!
#endif

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if !defined(NET_TS_HAS_THREADS)
typedef null_static_mutex static_mutex;
# define NET_TS_STATIC_MUTEX_INIT NET_TS_NULL_STATIC_MUTEX_INIT
#elif defined(NET_TS_WINDOWS)
typedef win_static_mutex static_mutex;
# define NET_TS_STATIC_MUTEX_INIT NET_TS_WIN_STATIC_MUTEX_INIT
#elif defined(NET_TS_HAS_PTHREADS)
typedef posix_static_mutex static_mutex;
# define NET_TS_STATIC_MUTEX_INIT NET_TS_POSIX_STATIC_MUTEX_INIT
#elif defined(NET_TS_HAS_STD_MUTEX_AND_CONDVAR)
typedef std_static_mutex static_mutex;
# define NET_TS_STATIC_MUTEX_INIT NET_TS_STD_STATIC_MUTEX_INIT
#endif

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_STATIC_MUTEX_HPP
