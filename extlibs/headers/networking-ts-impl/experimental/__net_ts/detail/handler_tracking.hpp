//
// detail/handler_tracking.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_HANDLER_TRACKING_HPP
#define NET_TS_DETAIL_HANDLER_TRACKING_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

class execution_context;

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#if defined(NET_TS_CUSTOM_HANDLER_TRACKING)
# include NET_TS_CUSTOM_HANDLER_TRACKING
#elif defined(NET_TS_ENABLE_HANDLER_TRACKING)
# include <system_error>
# include <experimental/__net_ts/detail/cstdint.hpp>
# include <experimental/__net_ts/detail/static_mutex.hpp>
# include <experimental/__net_ts/detail/tss_ptr.hpp>
#endif // defined(NET_TS_ENABLE_HANDLER_TRACKING)

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if defined(NET_TS_CUSTOM_HANDLER_TRACKING)

// The user-specified header must define the following macros:
// - NET_TS_INHERIT_TRACKED_HANDLER
// - NET_TS_ALSO_INHERIT_TRACKED_HANDLER
// - NET_TS_HANDLER_TRACKING_INIT
// - NET_TS_HANDLER_CREATION(args)
// - NET_TS_HANDLER_COMPLETION(args)
// - NET_TS_HANDLER_INVOCATION_BEGIN(args)
// - NET_TS_HANDLER_INVOCATION_END
// - NET_TS_HANDLER_OPERATION(args)
// - NET_TS_HANDLER_REACTOR_REGISTRATION(args)
// - NET_TS_HANDLER_REACTOR_DEREGISTRATION(args)
// - NET_TS_HANDLER_REACTOR_READ_EVENT
// - NET_TS_HANDLER_REACTOR_WRITE_EVENT
// - NET_TS_HANDLER_REACTOR_ERROR_EVENT
// - NET_TS_HANDLER_REACTOR_EVENTS(args)
// - NET_TS_HANDLER_REACTOR_OPERATION(args)

# if !defined(NET_TS_ENABLE_HANDLER_TRACKING)
#  define NET_TS_ENABLE_HANDLER_TRACKING 1
# endif /// !defined(NET_TS_ENABLE_HANDLER_TRACKING)

#elif defined(NET_TS_ENABLE_HANDLER_TRACKING)

class handler_tracking
{
public:
  class completion;

  // Base class for objects containing tracked handlers.
  class tracked_handler
  {
  private:
    // Only the handler_tracking class will have access to the id.
    friend class handler_tracking;
    friend class completion;
    uint64_t id_;

  protected:
    // Constructor initialises with no id.
    tracked_handler() : id_(0) {}

    // Prevent deletion through this type.
    ~tracked_handler() {}
  };

  // Initialise the tracking system.
  NET_TS_DECL static void init();

  // Record the creation of a tracked handler.
  NET_TS_DECL static void creation(
      execution_context& context, tracked_handler& h,
      const char* object_type, void* object,
      uintmax_t native_handle, const char* op_name);

  class completion
  {
  public:
    // Constructor records that handler is to be invoked with no arguments.
    NET_TS_DECL explicit completion(const tracked_handler& h);

    // Destructor records only when an exception is thrown from the handler, or
    // if the memory is being freed without the handler having been invoked.
    NET_TS_DECL ~completion();

    // Records that handler is to be invoked with no arguments.
    NET_TS_DECL void invocation_begin();

    // Records that handler is to be invoked with one arguments.
    NET_TS_DECL void invocation_begin(const std::error_code& ec);

    // Constructor records that handler is to be invoked with two arguments.
    NET_TS_DECL void invocation_begin(
        const std::error_code& ec, std::size_t bytes_transferred);

    // Constructor records that handler is to be invoked with two arguments.
    NET_TS_DECL void invocation_begin(
        const std::error_code& ec, int signal_number);

    // Constructor records that handler is to be invoked with two arguments.
    NET_TS_DECL void invocation_begin(
        const std::error_code& ec, const char* arg);

    // Record that handler invocation has ended.
    NET_TS_DECL void invocation_end();

  private:
    friend class handler_tracking;
    uint64_t id_;
    bool invoked_;
    completion* next_;
  };

  // Record an operation that is not directly associated with a handler.
  NET_TS_DECL static void operation(execution_context& context,
      const char* object_type, void* object,
      uintmax_t native_handle, const char* op_name);

  // Record that a descriptor has been registered with the reactor.
  NET_TS_DECL static void reactor_registration(execution_context& context,
      uintmax_t native_handle, uintmax_t registration);

  // Record that a descriptor has been deregistered from the reactor.
  NET_TS_DECL static void reactor_deregistration(execution_context& context,
      uintmax_t native_handle, uintmax_t registration);

  // Record a reactor-based operation that is associated with a handler.
  NET_TS_DECL static void reactor_events(execution_context& context,
      uintmax_t registration, unsigned events);

  // Record a reactor-based operation that is associated with a handler.
  NET_TS_DECL static void reactor_operation(
      const tracked_handler& h, const char* op_name,
      const std::error_code& ec);

  // Record a reactor-based operation that is associated with a handler.
  NET_TS_DECL static void reactor_operation(
      const tracked_handler& h, const char* op_name,
      const std::error_code& ec, std::size_t bytes_transferred);

  // Write a line of output.
  NET_TS_DECL static void write_line(const char* format, ...);

private:
  struct tracking_state;
  NET_TS_DECL static tracking_state* get_state();
};

# define NET_TS_INHERIT_TRACKED_HANDLER \
  : public std::experimental::net::v1::detail::handler_tracking::tracked_handler

# define NET_TS_ALSO_INHERIT_TRACKED_HANDLER \
  , public std::experimental::net::v1::detail::handler_tracking::tracked_handler

# define NET_TS_HANDLER_TRACKING_INIT \
  std::experimental::net::v1::detail::handler_tracking::init()

# define NET_TS_HANDLER_CREATION(args) \
  std::experimental::net::v1::detail::handler_tracking::creation args

# define NET_TS_HANDLER_COMPLETION(args) \
  std::experimental::net::v1::detail::handler_tracking::completion tracked_completion args

# define NET_TS_HANDLER_INVOCATION_BEGIN(args) \
  tracked_completion.invocation_begin args

# define NET_TS_HANDLER_INVOCATION_END \
  tracked_completion.invocation_end()

# define NET_TS_HANDLER_OPERATION(args) \
  std::experimental::net::v1::detail::handler_tracking::operation args

# define NET_TS_HANDLER_REACTOR_REGISTRATION(args) \
  std::experimental::net::v1::detail::handler_tracking::reactor_registration args

# define NET_TS_HANDLER_REACTOR_DEREGISTRATION(args) \
  std::experimental::net::v1::detail::handler_tracking::reactor_deregistration args

# define NET_TS_HANDLER_REACTOR_READ_EVENT 1
# define NET_TS_HANDLER_REACTOR_WRITE_EVENT 2
# define NET_TS_HANDLER_REACTOR_ERROR_EVENT 4

# define NET_TS_HANDLER_REACTOR_EVENTS(args) \
  std::experimental::net::v1::detail::handler_tracking::reactor_events args

# define NET_TS_HANDLER_REACTOR_OPERATION(args) \
  std::experimental::net::v1::detail::handler_tracking::reactor_operation args

#else // defined(NET_TS_ENABLE_HANDLER_TRACKING)

# define NET_TS_INHERIT_TRACKED_HANDLER
# define NET_TS_ALSO_INHERIT_TRACKED_HANDLER
# define NET_TS_HANDLER_TRACKING_INIT (void)0
# define NET_TS_HANDLER_CREATION(args) (void)0
# define NET_TS_HANDLER_COMPLETION(args) (void)0
# define NET_TS_HANDLER_INVOCATION_BEGIN(args) (void)0
# define NET_TS_HANDLER_INVOCATION_END (void)0
# define NET_TS_HANDLER_OPERATION(args) (void)0
# define NET_TS_HANDLER_REACTOR_REGISTRATION(args) (void)0
# define NET_TS_HANDLER_REACTOR_DEREGISTRATION(args) (void)0
# define NET_TS_HANDLER_REACTOR_READ_EVENT 0
# define NET_TS_HANDLER_REACTOR_WRITE_EVENT 0
# define NET_TS_HANDLER_REACTOR_ERROR_EVENT 0
# define NET_TS_HANDLER_REACTOR_EVENTS(args) (void)0
# define NET_TS_HANDLER_REACTOR_OPERATION(args) (void)0

#endif // defined(NET_TS_ENABLE_HANDLER_TRACKING)

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/handler_tracking.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_DETAIL_HANDLER_TRACKING_HPP
