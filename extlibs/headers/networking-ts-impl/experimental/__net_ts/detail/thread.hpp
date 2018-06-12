//
// detail/thread.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_THREAD_HPP
#define NET_TS_DETAIL_THREAD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_THREADS)
# include <experimental/__net_ts/detail/null_thread.hpp>
#elif defined(NET_TS_WINDOWS)
# if defined(UNDER_CE)
#  include <experimental/__net_ts/detail/wince_thread.hpp>
# elif defined(NET_TS_WINDOWS_APP)
#  include <experimental/__net_ts/detail/winapp_thread.hpp>
# else
#  include <experimental/__net_ts/detail/win_thread.hpp>
# endif
#elif defined(NET_TS_HAS_PTHREADS)
# include <experimental/__net_ts/detail/posix_thread.hpp>
#elif defined(NET_TS_HAS_STD_THREAD)
# include <experimental/__net_ts/detail/std_thread.hpp>
#else
# error Only Windows, POSIX and std::thread are supported!
#endif

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if !defined(NET_TS_HAS_THREADS)
typedef null_thread thread;
#elif defined(NET_TS_WINDOWS)
# if defined(UNDER_CE)
typedef wince_thread thread;
# elif defined(NET_TS_WINDOWS_APP)
typedef winapp_thread thread;
# else
typedef win_thread thread;
# endif
#elif defined(NET_TS_HAS_PTHREADS)
typedef posix_thread thread;
#elif defined(NET_TS_HAS_STD_THREAD)
typedef std_thread thread;
#endif

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_THREAD_HPP
