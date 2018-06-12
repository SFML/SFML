//
// system_context.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_SYSTEM_CONTEXT_HPP
#define NET_TS_SYSTEM_CONTEXT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/scheduler.hpp>
#include <experimental/__net_ts/detail/thread_group.hpp>
#include <experimental/__net_ts/execution_context.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

class system_executor;

/// The executor context for the system executor.
class system_context : public execution_context
{
public:
  /// The executor type associated with the context.
  typedef system_executor executor_type;

  /// Destructor shuts down all threads in the system thread pool.
  NET_TS_DECL ~system_context();

  /// Obtain an executor for the context.
  executor_type get_executor() NET_TS_NOEXCEPT;

  /// Signal all threads in the system thread pool to stop.
  NET_TS_DECL void stop();

  /// Determine whether the system thread pool has been stopped.
  NET_TS_DECL bool stopped() const NET_TS_NOEXCEPT;

  /// Join all threads in the system thread pool.
  NET_TS_DECL void join();

#if defined(GENERATING_DOCUMENTATION)
private:
#endif // defined(GENERATING_DOCUMENTATION)
  // Constructor creates all threads in the system thread pool.
  NET_TS_DECL system_context();

private:
  friend class system_executor;

  struct thread_function;

  // The underlying scheduler.
  detail::scheduler& scheduler_;

  // The threads in the system thread pool.
  detail::thread_group threads_;
};

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#include <experimental/__net_ts/impl/system_context.hpp>
#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/impl/system_context.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_SYSTEM_CONTEXT_HPP
