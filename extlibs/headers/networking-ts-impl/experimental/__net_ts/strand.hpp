//
// strand.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_STRAND_HPP
#define NET_TS_STRAND_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/strand_executor_service.hpp>
#include <experimental/__net_ts/detail/type_traits.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

/// Provides serialised function invocation for any executor type.
template <typename Executor>
class strand
{
public:
  /// The type of the underlying executor.
  typedef Executor inner_executor_type;

  /// Default constructor.
  /**
   * This constructor is only valid if the underlying executor type is default
   * constructible.
   */
  strand()
    : executor_(),
      impl_(use_service<detail::strand_executor_service>(
            executor_.context()).create_implementation())
  {
  }

  /// Construct a strand for the specified executor.
  explicit strand(const Executor& e)
    : executor_(e),
      impl_(use_service<detail::strand_executor_service>(
            executor_.context()).create_implementation())
  {
  }

  /// Copy constructor.
  strand(const strand& other) NET_TS_NOEXCEPT
    : executor_(other.executor_),
      impl_(other.impl_)
  {
  }

  /// Converting constructor.
  /**
   * This constructor is only valid if the @c OtherExecutor type is convertible
   * to @c Executor.
   */
  template <class OtherExecutor>
  strand(
      const strand<OtherExecutor>& other) NET_TS_NOEXCEPT
    : executor_(other.executor_),
      impl_(other.impl_)
  {
  }

  /// Assignment operator.
  strand& operator=(const strand& other) NET_TS_NOEXCEPT
  {
    executor_ = other.executor_;
    impl_ = other.impl_;
    return *this;
  }

  /// Converting assignment operator.
  /**
   * This assignment operator is only valid if the @c OtherExecutor type is
   * convertible to @c Executor.
   */
  template <class OtherExecutor>
  strand& operator=(
      const strand<OtherExecutor>& other) NET_TS_NOEXCEPT
  {
    executor_ = other.executor_;
    impl_ = other.impl_;
    return *this;
  }

#if defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Move constructor.
  strand(strand&& other) NET_TS_NOEXCEPT
    : executor_(NET_TS_MOVE_CAST(Executor)(other.executor_)),
      impl_(NET_TS_MOVE_CAST(implementation_type)(other.impl_))
  {
  }

  /// Converting move constructor.
  /**
   * This constructor is only valid if the @c OtherExecutor type is convertible
   * to @c Executor.
   */
  template <class OtherExecutor>
  strand(strand<OtherExecutor>&& other) NET_TS_NOEXCEPT
    : executor_(NET_TS_MOVE_CAST(OtherExecutor)(other)),
      impl_(NET_TS_MOVE_CAST(implementation_type)(other.impl_))
  {
  }

  /// Move assignment operator.
  strand& operator=(strand&& other) NET_TS_NOEXCEPT
  {
    executor_ = NET_TS_MOVE_CAST(Executor)(other);
    impl_ = NET_TS_MOVE_CAST(implementation_type)(other.impl_);
    return *this;
  }

  /// Converting move assignment operator.
  /**
   * This assignment operator is only valid if the @c OtherExecutor type is
   * convertible to @c Executor.
   */
  template <class OtherExecutor>
  strand& operator=(
      const strand<OtherExecutor>&& other) NET_TS_NOEXCEPT
  {
    executor_ = NET_TS_MOVE_CAST(OtherExecutor)(other);
    impl_ = NET_TS_MOVE_CAST(implementation_type)(other.impl_);
    return *this;
  }
#endif // defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Destructor.
  ~strand()
  {
  }

  /// Obtain the underlying executor.
  inner_executor_type get_inner_executor() const NET_TS_NOEXCEPT
  {
    return executor_;
  }

  /// Obtain the underlying execution context.
  execution_context& context() const NET_TS_NOEXCEPT
  {
    return executor_.context();
  }

  /// Inform the strand that it has some outstanding work to do.
  /**
   * The strand delegates this call to its underlying executor.
   */
  void on_work_started() const NET_TS_NOEXCEPT
  {
    executor_.on_work_started();
  }

  /// Inform the strand that some work is no longer outstanding.
  /**
   * The strand delegates this call to its underlying executor.
   */
  void on_work_finished() const NET_TS_NOEXCEPT
  {
    executor_.on_work_finished();
  }

  /// Request the strand to invoke the given function object.
  /**
   * This function is used to ask the strand to execute the given function
   * object on its underlying executor. The function object will be executed
   * inside this function if the strand is not otherwise busy and if the
   * underlying executor's @c dispatch() function is also able to execute the
   * function before returning.
   *
   * @param f The function object to be called. The executor will make
   * a copy of the handler object as required. The function signature of the
   * function object must be: @code void function(); @endcode
   *
   * @param a An allocator that may be used by the executor to allocate the
   * internal storage needed for function invocation.
   */
  template <typename Function, typename Allocator>
  void dispatch(NET_TS_MOVE_ARG(Function) f, const Allocator& a) const
  {
    detail::strand_executor_service::dispatch(impl_,
        executor_, NET_TS_MOVE_CAST(Function)(f), a);
  }

  /// Request the strand to invoke the given function object.
  /**
   * This function is used to ask the executor to execute the given function
   * object. The function object will never be executed inside this function.
   * Instead, it will be scheduled by the underlying executor's defer function.
   *
   * @param f The function object to be called. The executor will make
   * a copy of the handler object as required. The function signature of the
   * function object must be: @code void function(); @endcode
   *
   * @param a An allocator that may be used by the executor to allocate the
   * internal storage needed for function invocation.
   */
  template <typename Function, typename Allocator>
  void post(NET_TS_MOVE_ARG(Function) f, const Allocator& a) const
  {
    detail::strand_executor_service::post(impl_,
        executor_, NET_TS_MOVE_CAST(Function)(f), a);
  }

  /// Request the strand to invoke the given function object.
  /**
   * This function is used to ask the executor to execute the given function
   * object. The function object will never be executed inside this function.
   * Instead, it will be scheduled by the underlying executor's defer function.
   *
   * @param f The function object to be called. The executor will make
   * a copy of the handler object as required. The function signature of the
   * function object must be: @code void function(); @endcode
   *
   * @param a An allocator that may be used by the executor to allocate the
   * internal storage needed for function invocation.
   */
  template <typename Function, typename Allocator>
  void defer(NET_TS_MOVE_ARG(Function) f, const Allocator& a) const
  {
    detail::strand_executor_service::defer(impl_,
        executor_, NET_TS_MOVE_CAST(Function)(f), a);
  }

  /// Determine whether the strand is running in the current thread.
  /**
   * @return @c true if the current thread is executing a function that was
   * submitted to the strand using post(), dispatch() or defer(). Otherwise
   * returns @c false.
   */
  bool running_in_this_thread() const NET_TS_NOEXCEPT
  {
    return detail::strand_executor_service::running_in_this_thread(impl_);
  }

  /// Compare two strands for equality.
  /**
   * Two strands are equal if they refer to the same ordered, non-concurrent
   * state.
   */
  friend bool operator==(const strand& a, const strand& b) NET_TS_NOEXCEPT
  {
    return a.impl_ == b.impl_;
  }

  /// Compare two strands for inequality.
  /**
   * Two strands are equal if they refer to the same ordered, non-concurrent
   * state.
   */
  friend bool operator!=(const strand& a, const strand& b) NET_TS_NOEXCEPT
  {
    return a.impl_ != b.impl_;
  }

private:
  Executor executor_;
  typedef detail::strand_executor_service::implementation_type
    implementation_type;
  implementation_type impl_;
};

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

// If both io_context.hpp and strand.hpp have been included, automatically
// include the header file needed for the io_context::strand class.
#endif // NET_TS_STRAND_HPP
