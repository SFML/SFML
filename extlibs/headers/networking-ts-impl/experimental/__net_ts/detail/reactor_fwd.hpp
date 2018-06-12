//
// detail/reactor_fwd.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_REACTOR_FWD_HPP
#define NET_TS_DETAIL_REACTOR_FWD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if defined(NET_TS_HAS_IOCP) || defined(NET_TS_WINDOWS_RUNTIME)
typedef class null_reactor reactor;
#elif defined(NET_TS_HAS_IOCP)
typedef class select_reactor reactor;
#elif defined(NET_TS_HAS_EPOLL)
typedef class epoll_reactor reactor;
#elif defined(NET_TS_HAS_KQUEUE)
typedef class kqueue_reactor reactor;
#elif defined(NET_TS_HAS_DEV_POLL)
typedef class dev_poll_reactor reactor;
#else
typedef class select_reactor reactor;
#endif

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_REACTOR_FWD_HPP
