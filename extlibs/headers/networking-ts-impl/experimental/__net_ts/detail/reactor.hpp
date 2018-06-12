//
// detail/reactor.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_REACTOR_HPP
#define NET_TS_DETAIL_REACTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/reactor_fwd.hpp>

#if defined(NET_TS_HAS_EPOLL)
# include <experimental/__net_ts/detail/epoll_reactor.hpp>
#elif defined(NET_TS_HAS_KQUEUE)
# include <experimental/__net_ts/detail/kqueue_reactor.hpp>
#elif defined(NET_TS_HAS_DEV_POLL)
# include <experimental/__net_ts/detail/dev_poll_reactor.hpp>
#elif defined(NET_TS_HAS_IOCP) || defined(NET_TS_WINDOWS_RUNTIME)
# include <experimental/__net_ts/detail/null_reactor.hpp>
#else
# include <experimental/__net_ts/detail/select_reactor.hpp>
#endif

#endif // NET_TS_DETAIL_REACTOR_HPP
