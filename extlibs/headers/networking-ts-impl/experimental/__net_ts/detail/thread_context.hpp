//
// detail/thread_context.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_THREAD_CONTEXT_HPP
#define NET_TS_DETAIL_THREAD_CONTEXT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <climits>
#include <cstddef>
#include <experimental/__net_ts/detail/call_stack.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class thread_info_base;

// Base class for things that manage threads (scheduler, win_iocp_io_context).
class thread_context
{
public:
  // Per-thread call stack to track the state of each thread in the context.
  typedef call_stack<thread_context, thread_info_base> thread_call_stack;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_THREAD_CONTEXT_HPP
