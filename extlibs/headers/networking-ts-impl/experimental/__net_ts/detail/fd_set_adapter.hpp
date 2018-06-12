//
// detail/fd_set_adapter.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_FD_SET_ADAPTER_HPP
#define NET_TS_DETAIL_FD_SET_ADAPTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_WINDOWS_RUNTIME)

#include <experimental/__net_ts/detail/posix_fd_set_adapter.hpp>
#include <experimental/__net_ts/detail/win_fd_set_adapter.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
typedef win_fd_set_adapter fd_set_adapter;
#else
typedef posix_fd_set_adapter fd_set_adapter;
#endif

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // !defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_FD_SET_ADAPTER_HPP
