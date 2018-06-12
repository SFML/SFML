//
// bind_executor.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_BIND_EXECUTOR_HPP
#define NET_TS_BIND_EXECUTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/type_traits.hpp>
#include <experimental/__net_ts/detail/variadic_templates.hpp>
#include <experimental/__net_ts/associated_executor.hpp>
#include <experimental/__net_ts/associated_allocator.hpp>
#include <experimental/__net_ts/async_result.hpp>
#include <experimental/__net_ts/execution_context.hpp>
#include <experimental/__net_ts/is_executor.hpp>
#include <experimental/__net_ts/uses_executor.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename T>
struct executor_binder_check
{
  typedef void type;
};

// Helper to automatically define nested typedef result_type.

template <typename T, typename = void>
struct executor_binder_result_type
{
protected:
  typedef void result_type_or_void;
};

template <typename T>
struct executor_binder_result_type<T,
  typename executor_binder_check<typename T::result_type>::type>
{
  typedef typename T::result_type result_type;
protected:
  typedef result_type result_type_or_void;
};

template <typename R>
struct executor_binder_result_type<R(*)()>
{
  typedef R result_type;
protected:
  typedef result_type result_type_or_void;
};

template <typename R>
struct executor_binder_result_type<R(&)()>
{
  typedef R result_type;
protected:
  typedef result_type result_type_or_void;
};

template <typename R, typename A1>
struct executor_binder_result_type<R(*)(A1)>
{
  typedef R result_type;
protected:
  typedef result_type result_type_or_void;
};

template <typename R, typename A1>
struct executor_binder_result_type<R(&)(A1)>
{
  typedef R result_type;
protected:
  typedef result_type result_type_or_void;
};

template <typename R, typename A1, typename A2>
struct executor_binder_result_type<R(*)(A1, A2)>
{
  typedef R result_type;
protected:
  typedef result_type result_type_or_void;
};

template <typename R, typename A1, typename A2>
struct executor_binder_result_type<R(&)(A1, A2)>
{
  typedef R result_type;
protected:
  typedef result_type result_type_or_void;
};

// Helper to automatically define nested typedef argument_type.

template <typename T, typename = void>
struct executor_binder_argument_type {};

template <typename T>
struct executor_binder_argument_type<T,
  typename executor_binder_check<typename T::argument_type>::type>
{
  typedef typename T::argument_type argument_type;
};

template <typename R, typename A1>
struct executor_binder_argument_type<R(*)(A1)>
{
  typedef A1 argument_type;
};

template <typename R, typename A1>
struct executor_binder_argument_type<R(&)(A1)>
{
  typedef A1 argument_type;
};

// Helper to automatically define nested typedefs first_argument_type and
// second_argument_type.

template <typename T, typename = void>
struct executor_binder_argument_types {};

template <typename T>
struct executor_binder_argument_types<T,
  typename executor_binder_check<typename T::first_argument_type>::type>
{
  typedef typename T::first_argument_type first_argument_type;
  typedef typename T::second_argument_type second_argument_type;
};

template <typename R, typename A1, typename A2>
struct executor_binder_argument_type<R(*)(A1, A2)>
{
  typedef A1 first_argument_type;
  typedef A2 second_argument_type;
};

template <typename R, typename A1, typename A2>
struct executor_binder_argument_type<R(&)(A1, A2)>
{
  typedef A1 first_argument_type;
  typedef A2 second_argument_type;
};

// Helper to:
// - Apply the empty base optimisation to the executor.
// - Perform uses_executor construction of the target type, if required.

template <typename T, typename Executor, bool UsesExecutor>
class executor_binder_base;

template <typename T, typename Executor>
class executor_binder_base<T, Executor, true>
  : protected Executor
{
protected:
  template <typename E, typename U>
  executor_binder_base(NET_TS_MOVE_ARG(E) e, NET_TS_MOVE_ARG(U) u)
    : executor_(NET_TS_MOVE_CAST(E)(e)),
      target_(executor_arg_t(), executor_, NET_TS_MOVE_CAST(U)(u))
  {
  }

  Executor executor_;
  T target_;
};

template <typename T, typename Executor>
class executor_binder_base<T, Executor, false>
{
protected:
  template <typename E, typename U>
  executor_binder_base(NET_TS_MOVE_ARG(E) e, NET_TS_MOVE_ARG(U) u)
    : executor_(NET_TS_MOVE_CAST(E)(e)),
      target_(NET_TS_MOVE_CAST(U)(u))
  {
  }

  Executor executor_;
  T target_;
};

// Helper to enable SFINAE on zero-argument operator() below.

template <typename T, typename = void>
struct executor_binder_result_of0
{
  typedef void type;
};

template <typename T>
struct executor_binder_result_of0<T,
  typename executor_binder_check<typename result_of<T()>::type>::type>
{
  typedef typename result_of<T()>::type type;
};

} // namespace detail

/// A call wrapper type to bind an executor of type @c Executor to an object of
/// type @c T.
template <typename T, typename Executor>
class executor_binder
#if !defined(GENERATING_DOCUMENTATION)
  : public detail::executor_binder_result_type<T>,
    public detail::executor_binder_argument_type<T>,
    public detail::executor_binder_argument_types<T>,
    private detail::executor_binder_base<
      T, Executor, uses_executor<T, Executor>::value>
#endif // !defined(GENERATING_DOCUMENTATION)
{
public:
  /// The type of the target object.
  typedef T target_type;

  /// The type of the associated executor.
  typedef Executor executor_type;

#if defined(GENERATING_DOCUMENTATION)
  /// The return type if a function.
  /**
   * The type of @c result_type is based on the type @c T of the wrapper's
   * target object:
   *
   * @li if @c T is a pointer to function type, @c result_type is a synonym for
   * the return type of @c T;
   *
   * @li if @c T is a class type with a member type @c result_type, then @c
   * result_type is a synonym for @c T::result_type;
   *
   * @li otherwise @c result_type is not defined.
   */
  typedef see_below result_type;

  /// The type of the function's argument.
  /**
   * The type of @c argument_type is based on the type @c T of the wrapper's
   * target object:
   *
   * @li if @c T is a pointer to a function type accepting a single argument,
   * @c argument_type is a synonym for the return type of @c T;
   *
   * @li if @c T is a class type with a member type @c argument_type, then @c
   * argument_type is a synonym for @c T::argument_type;
   *
   * @li otherwise @c argument_type is not defined.
   */
  typedef see_below argument_type;

  /// The type of the function's first argument.
  /**
   * The type of @c first_argument_type is based on the type @c T of the
   * wrapper's target object:
   *
   * @li if @c T is a pointer to a function type accepting two arguments, @c
   * first_argument_type is a synonym for the return type of @c T;
   *
   * @li if @c T is a class type with a member type @c first_argument_type,
   * then @c first_argument_type is a synonym for @c T::first_argument_type;
   *
   * @li otherwise @c first_argument_type is not defined.
   */
  typedef see_below first_argument_type;

  /// The type of the function's second argument.
  /**
   * The type of @c second_argument_type is based on the type @c T of the
   * wrapper's target object:
   *
   * @li if @c T is a pointer to a function type accepting two arguments, @c
   * second_argument_type is a synonym for the return type of @c T;
   *
   * @li if @c T is a class type with a member type @c first_argument_type,
   * then @c second_argument_type is a synonym for @c T::second_argument_type;
   *
   * @li otherwise @c second_argument_type is not defined.
   */
  typedef see_below second_argument_type;
#endif // defined(GENERATING_DOCUMENTATION)

  /// Construct an executor wrapper for the specified object.
  /**
   * This constructor is only valid if the type @c T is constructible from type
   * @c U.
   */
  template <typename U>
  executor_binder(executor_arg_t, const executor_type& e,
      NET_TS_MOVE_ARG(U) u)
    : base_type(e, NET_TS_MOVE_CAST(U)(u))
  {
  }

  /// Copy constructor.
  executor_binder(const executor_binder& other)
    : base_type(other.get_executor(), other.get())
  {
  }

  /// Construct a copy, but specify a different executor.
  executor_binder(executor_arg_t, const executor_type& e,
      const executor_binder& other)
    : base_type(e, other.get())
  {
  }

  /// Construct a copy of a different executor wrapper type.
  /**
   * This constructor is only valid if the @c Executor type is constructible
   * from type @c OtherExecutor, and the type @c T is constructible from type
   * @c U.
   */
  template <typename U, typename OtherExecutor>
  executor_binder(const executor_binder<U, OtherExecutor>& other)
    : base_type(other.get_executor(), other.get())
  {
  }

  /// Construct a copy of a different executor wrapper type, but specify a
  /// different executor.
  /**
   * This constructor is only valid if the type @c T is constructible from type
   * @c U.
   */
  template <typename U, typename OtherExecutor>
  executor_binder(executor_arg_t, const executor_type& e,
      const executor_binder<U, OtherExecutor>& other)
    : base_type(e, other.get())
  {
  }

#if defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Move constructor.
  executor_binder(executor_binder&& other)
    : base_type(NET_TS_MOVE_CAST(executor_type)(other.get_executor()),
        NET_TS_MOVE_CAST(T)(other.get()))
  {
  }

  /// Move construct the target object, but specify a different executor.
  executor_binder(executor_arg_t, const executor_type& e,
      executor_binder&& other)
    : base_type(e, NET_TS_MOVE_CAST(T)(other.get()))
  {
  }

  /// Move construct from a different executor wrapper type.
  template <typename U, typename OtherExecutor>
  executor_binder(executor_binder<U, OtherExecutor>&& other)
    : base_type(NET_TS_MOVE_CAST(OtherExecutor)(other.get_executor()),
        NET_TS_MOVE_CAST(U)(other.get()))
  {
  }

  /// Move construct from a different executor wrapper type, but specify a
  /// different executor.
  template <typename U, typename OtherExecutor>
  executor_binder(executor_arg_t, const executor_type& e,
      executor_binder<U, OtherExecutor>&& other)
    : base_type(e, NET_TS_MOVE_CAST(U)(other.get()))
  {
  }

#endif // defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Destructor.
  ~executor_binder()
  {
  }

  /// Obtain a reference to the target object.
  target_type& get() NET_TS_NOEXCEPT
  {
    return this->target_;
  }

  /// Obtain a reference to the target object.
  const target_type& get() const NET_TS_NOEXCEPT
  {
    return this->target_;
  }

  /// Obtain the associated executor.
  executor_type get_executor() const NET_TS_NOEXCEPT
  {
    return this->executor_;
  }

#if defined(GENERATING_DOCUMENTATION)

  template <typename... Args> auto operator()(Args&& ...);
  template <typename... Args> auto operator()(Args&& ...) const;

#elif defined(NET_TS_HAS_VARIADIC_TEMPLATES)

  /// Forwarding function call operator.
  template <typename... Args>
  typename result_of<T(Args...)>::type operator()(
      NET_TS_MOVE_ARG(Args)... args)
  {
    return this->target_(NET_TS_MOVE_CAST(Args)(args)...);
  }

  /// Forwarding function call operator.
  template <typename... Args>
  typename result_of<T(Args...)>::type operator()(
      NET_TS_MOVE_ARG(Args)... args) const
  {
    return this->target_(NET_TS_MOVE_CAST(Args)(args)...);
  }

#elif defined(NET_TS_HAS_STD_TYPE_TRAITS) && !defined(_MSC_VER)

  typename detail::executor_binder_result_of0<T>::type operator()()
  {
    return this->target_();
  }

  typename detail::executor_binder_result_of0<T>::type operator()() const
  {
    return this->target_();
  }

#define NET_TS_PRIVATE_BIND_EXECUTOR_CALL_DEF(n) \
  template <NET_TS_VARIADIC_TPARAMS(n)> \
  typename result_of<T(NET_TS_VARIADIC_TARGS(n))>::type operator()( \
      NET_TS_VARIADIC_MOVE_PARAMS(n)) \
  { \
    return this->target_(NET_TS_VARIADIC_MOVE_ARGS(n)); \
  } \
  \
  template <NET_TS_VARIADIC_TPARAMS(n)> \
  typename result_of<T(NET_TS_VARIADIC_TARGS(n))>::type operator()( \
      NET_TS_VARIADIC_MOVE_PARAMS(n)) const \
  { \
    return this->target_(NET_TS_VARIADIC_MOVE_ARGS(n)); \
  } \
  /**/
  NET_TS_VARIADIC_GENERATE(NET_TS_PRIVATE_BIND_EXECUTOR_CALL_DEF)
#undef NET_TS_PRIVATE_BIND_EXECUTOR_CALL_DEF

#else // defined(NET_TS_HAS_STD_TYPE_TRAITS) && !defined(_MSC_VER)

  typedef typename detail::executor_binder_result_type<T>::result_type_or_void
    result_type_or_void;

  result_type_or_void operator()()
  {
    return this->target_();
  }

  result_type_or_void operator()() const
  {
    return this->target_();
  }

#define NET_TS_PRIVATE_BIND_EXECUTOR_CALL_DEF(n) \
  template <NET_TS_VARIADIC_TPARAMS(n)> \
  result_type_or_void operator()( \
      NET_TS_VARIADIC_MOVE_PARAMS(n)) \
  { \
    return this->target_(NET_TS_VARIADIC_MOVE_ARGS(n)); \
  } \
  \
  template <NET_TS_VARIADIC_TPARAMS(n)> \
  result_type_or_void operator()( \
      NET_TS_VARIADIC_MOVE_PARAMS(n)) const \
  { \
    return this->target_(NET_TS_VARIADIC_MOVE_ARGS(n)); \
  } \
  /**/
  NET_TS_VARIADIC_GENERATE(NET_TS_PRIVATE_BIND_EXECUTOR_CALL_DEF)
#undef NET_TS_PRIVATE_BIND_EXECUTOR_CALL_DEF

#endif // defined(NET_TS_HAS_STD_TYPE_TRAITS) && !defined(_MSC_VER)

private:
  typedef detail::executor_binder_base<T, Executor,
    uses_executor<T, Executor>::value> base_type;
};

/// Associate an object of type @c T with an executor of type @c Executor.
template <typename Executor, typename T>
inline executor_binder<typename decay<T>::type, Executor>
bind_executor(const Executor& ex, NET_TS_MOVE_ARG(T) t,
    typename enable_if<is_executor<Executor>::value>::type* = 0)
{
  return executor_binder<typename decay<T>::type, Executor>(
      executor_arg_t(), ex, NET_TS_MOVE_CAST(T)(t));
}

/// Associate an object of type @c T with an execution context's executor.
template <typename ExecutionContext, typename T>
inline executor_binder<typename decay<T>::type,
  typename ExecutionContext::executor_type>
bind_executor(ExecutionContext& ctx, NET_TS_MOVE_ARG(T) t,
    typename enable_if<is_convertible<
      ExecutionContext&, execution_context&>::value>::type* = 0)
{
  return executor_binder<typename decay<T>::type,
    typename ExecutionContext::executor_type>(
      executor_arg_t(), ctx.get_executor(), NET_TS_MOVE_CAST(T)(t));
}

#if !defined(GENERATING_DOCUMENTATION)

template <typename T, typename Executor>
struct uses_executor<executor_binder<T, Executor>, Executor>
  : true_type {};

template <typename T, typename Executor, typename Signature>
class async_result<executor_binder<T, Executor>, Signature>
{
public:
  typedef executor_binder<
    typename async_result<T, Signature>::completion_handler_type, Executor>
      completion_handler_type;

  typedef typename async_result<T, Signature>::return_type return_type;

  explicit async_result(executor_binder<T, Executor>& b)
    : target_(b.get())
  {
  }

  return_type get()
  {
    return target_.get();
  }

private:
  async_result(const async_result&) NET_TS_DELETED;
  async_result& operator=(const async_result&) NET_TS_DELETED;

  async_result<T, Signature> target_;
};

template <typename T, typename Executor, typename Allocator>
struct associated_allocator<executor_binder<T, Executor>, Allocator>
{
  typedef typename associated_allocator<T, Allocator>::type type;

  static type get(const executor_binder<T, Executor>& b,
      const Allocator& a = Allocator()) NET_TS_NOEXCEPT
  {
    return associated_allocator<T, Allocator>::get(b.get(), a);
  }
};

template <typename T, typename Executor, typename Executor1>
struct associated_executor<executor_binder<T, Executor>, Executor1>
{
  typedef Executor type;

  static type get(const executor_binder<T, Executor>& b,
      const Executor1& = Executor1()) NET_TS_NOEXCEPT
  {
    return b.get_executor();
  }
};

#endif // !defined(GENERATING_DOCUMENTATION)

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_BIND_EXECUTOR_HPP
