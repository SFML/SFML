//
// detail/resolver_service_base.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_RESOLVER_SERVICE_BASE_HPP
#define NET_TS_DETAIL_RESOLVER_SERVICE_BASE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/error.hpp>
#include <experimental/__net_ts/executor_work_guard.hpp>
#include <experimental/__net_ts/io_context.hpp>
#include <experimental/__net_ts/detail/mutex.hpp>
#include <experimental/__net_ts/detail/noncopyable.hpp>
#include <experimental/__net_ts/detail/resolve_op.hpp>
#include <experimental/__net_ts/detail/socket_ops.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>
#include <experimental/__net_ts/detail/scoped_ptr.hpp>
#include <experimental/__net_ts/detail/thread.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class resolver_service_base
{
public:
  // The implementation type of the resolver. A cancellation token is used to
  // indicate to the background thread that the operation has been cancelled.
  typedef socket_ops::shared_cancel_token_type implementation_type;

  // Constructor.
  NET_TS_DECL resolver_service_base(std::experimental::net::v1::io_context& io_context);

  // Destructor.
  NET_TS_DECL ~resolver_service_base();

  // Destroy all user-defined handler objects owned by the service.
  NET_TS_DECL void base_shutdown();

  // Perform any fork-related housekeeping.
  NET_TS_DECL void base_notify_fork(
      std::experimental::net::v1::io_context::fork_event fork_ev);

  // Construct a new resolver implementation.
  NET_TS_DECL void construct(implementation_type& impl);

  // Destroy a resolver implementation.
  NET_TS_DECL void destroy(implementation_type&);

  // Move-construct a new resolver implementation.
  NET_TS_DECL void move_construct(implementation_type& impl,
      implementation_type& other_impl);

  // Move-assign from another resolver implementation.
  NET_TS_DECL void move_assign(implementation_type& impl,
      resolver_service_base& other_service,
      implementation_type& other_impl);

  // Cancel pending asynchronous operations.
  NET_TS_DECL void cancel(implementation_type& impl);

protected:
  // Helper function to start an asynchronous resolve operation.
  NET_TS_DECL void start_resolve_op(resolve_op* op);

#if !defined(NET_TS_WINDOWS_RUNTIME)
  // Helper class to perform exception-safe cleanup of addrinfo objects.
  class auto_addrinfo
    : private std::experimental::net::v1::detail::noncopyable
  {
  public:
    explicit auto_addrinfo(std::experimental::net::v1::detail::addrinfo_type* ai)
      : ai_(ai)
    {
    }

    ~auto_addrinfo()
    {
      if (ai_)
        socket_ops::freeaddrinfo(ai_);
    }

    operator std::experimental::net::v1::detail::addrinfo_type*()
    {
      return ai_;
    }

  private:
    std::experimental::net::v1::detail::addrinfo_type* ai_;
  };
#endif // !defined(NET_TS_WINDOWS_RUNTIME)

  // Helper class to run the work io_context in a thread.
  class work_io_context_runner;

  // Start the work thread if it's not already running.
  NET_TS_DECL void start_work_thread();

  // The io_context implementation used to post completions.
  io_context_impl& io_context_impl_;

private:
  // Mutex to protect access to internal data.
  std::experimental::net::v1::detail::mutex mutex_;

  // Private io_context used for performing asynchronous host resolution.
  std::experimental::net::v1::detail::scoped_ptr<std::experimental::net::v1::io_context> work_io_context_;

  // The work io_context implementation used to post completions.
  io_context_impl& work_io_context_impl_;

  // Work for the private io_context to perform.
  std::experimental::net::v1::executor_work_guard<
      std::experimental::net::v1::io_context::executor_type> work_;

  // Thread used for running the work io_context's run loop.
  std::experimental::net::v1::detail::scoped_ptr<std::experimental::net::v1::detail::thread> work_thread_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/resolver_service_base.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_DETAIL_RESOLVER_SERVICE_BASE_HPP
