//
// detail/mutex.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_MUTEX_HPP
#define NET_TS_DETAIL_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_THREADS)
# include <experimental/__net_ts/detail/null_mutex.hpp>
#elif defined(NET_TS_WINDOWS)
# include <experimental/__net_ts/detail/win_mutex.hpp>
#elif defined(NET_TS_HAS_PTHREADS)
# include <experimental/__net_ts/detail/posix_mutex.hpp>
#elif defined(NET_TS_HAS_STD_MUTEX_AND_CONDVAR)
# include <experimental/__net_ts/detail/std_mutex.hpp>
#else
# error Only Windows, POSIX and std::mutex are supported!
#endif

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if !defined(NET_TS_HAS_THREADS)
typedef null_mutex mutex;
#elif defined(NET_TS_WINDOWS)
typedef win_mutex mutex;
#elif defined(NET_TS_HAS_PTHREADS)
typedef posix_mutex mutex;
#elif defined(NET_TS_HAS_STD_MUTEX_AND_CONDVAR)
typedef std_mutex mutex;
#endif

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_MUTEX_HPP
