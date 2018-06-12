//
// detail/select_interrupter.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_SELECT_INTERRUPTER_HPP
#define NET_TS_DETAIL_SELECT_INTERRUPTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_WINDOWS_RUNTIME)

#if defined(NET_TS_WINDOWS) || defined(__CYGWIN__) || defined(__SYMBIAN32__)
# include <experimental/__net_ts/detail/socket_select_interrupter.hpp>
#elif defined(NET_TS_HAS_EVENTFD)
# include <experimental/__net_ts/detail/eventfd_select_interrupter.hpp>
#else
# include <experimental/__net_ts/detail/pipe_select_interrupter.hpp>
#endif

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if defined(NET_TS_WINDOWS) || defined(__CYGWIN__) || defined(__SYMBIAN32__)
typedef socket_select_interrupter select_interrupter;
#elif defined(NET_TS_HAS_EVENTFD)
typedef eventfd_select_interrupter select_interrupter;
#else
typedef pipe_select_interrupter select_interrupter;
#endif

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // !defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_SELECT_INTERRUPTER_HPP
