//
// detail/signal_blocker.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_SIGNAL_BLOCKER_HPP
#define NET_TS_DETAIL_SIGNAL_BLOCKER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_THREADS) || defined(NET_TS_WINDOWS) \
  || defined(NET_TS_WINDOWS_RUNTIME) \
  || defined(__CYGWIN__) || defined(__SYMBIAN32__)
# include <experimental/__net_ts/detail/null_signal_blocker.hpp>
#elif defined(NET_TS_HAS_PTHREADS)
# include <experimental/__net_ts/detail/posix_signal_blocker.hpp>
#else
# error Only Windows and POSIX are supported!
#endif

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if !defined(NET_TS_HAS_THREADS) || defined(NET_TS_WINDOWS) \
  || defined(NET_TS_WINDOWS_RUNTIME) \
  || defined(__CYGWIN__) || defined(__SYMBIAN32__)
typedef null_signal_blocker signal_blocker;
#elif defined(NET_TS_HAS_PTHREADS)
typedef posix_signal_blocker signal_blocker;
#endif

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_SIGNAL_BLOCKER_HPP
