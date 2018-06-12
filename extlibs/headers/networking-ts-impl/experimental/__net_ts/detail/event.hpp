//
// detail/event.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_EVENT_HPP
#define NET_TS_DETAIL_EVENT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_THREADS)
# include <experimental/__net_ts/detail/null_event.hpp>
#elif defined(NET_TS_WINDOWS)
# include <experimental/__net_ts/detail/win_event.hpp>
#elif defined(NET_TS_HAS_PTHREADS)
# include <experimental/__net_ts/detail/posix_event.hpp>
#elif defined(NET_TS_HAS_STD_MUTEX_AND_CONDVAR)
# include <experimental/__net_ts/detail/std_event.hpp>
#else
# error Only Windows, POSIX and std::condition_variable are supported!
#endif

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if !defined(NET_TS_HAS_THREADS)
typedef null_event event;
#elif defined(NET_TS_WINDOWS)
typedef win_event event;
#elif defined(NET_TS_HAS_PTHREADS)
typedef posix_event event;
#elif defined(NET_TS_HAS_STD_MUTEX_AND_CONDVAR)
typedef std_event event;
#endif

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_EVENT_HPP
