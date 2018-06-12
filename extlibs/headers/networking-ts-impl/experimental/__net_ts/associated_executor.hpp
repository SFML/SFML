//
// associated_executor.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_ASSOCIATED_EXECUTOR_HPP
#define NET_TS_ASSOCIATED_EXECUTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/type_traits.hpp>
#include <experimental/__net_ts/is_executor.hpp>
#include <experimental/__net_ts/system_executor.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename>
struct associated_executor_check
{
  typedef void type;
};

template <typename T, typename E, typename = void>
struct associated_executor_impl
{
  typedef E type;

  static type get(const T&, const E& e) NET_TS_NOEXCEPT
  {
    return e;
  }
};

template <typename T, typename E>
struct associated_executor_impl<T, E,
  typename associated_executor_check<typename T::executor_type>::type>
{
  typedef typename T::executor_type type;

  static type get(const T& t, const E&) NET_TS_NOEXCEPT
  {
    return t.get_executor();
  }
};

} // namespace detail

/// Traits type used to obtain the executor associated with an object.
/**
 * A program may specialise this traits type if the @c T template parameter in
 * the specialisation is a user-defined type. The template parameter @c
 * Executor shall be a type meeting the Executor requirements.
 *
 * Specialisations shall meet the following requirements, where @c t is a const
 * reference to an object of type @c T, and @c e is an object of type @c
 * Executor.
 *
 * @li Provide a nested typedef @c type that identifies a type meeting the
 * Executor requirements.
 *
 * @li Provide a noexcept static member function named @c get, callable as @c
 * get(t) and with return type @c type.
 *
 * @li Provide a noexcept static member function named @c get, callable as @c
 * get(t,e) and with return type @c type.
 */
template <typename T, typename Executor = system_executor>
struct associated_executor
{
  /// If @c T has a nested type @c executor_type, <tt>T::executor_type</tt>.
  /// Otherwise @c Executor.
#if defined(GENERATING_DOCUMENTATION)
  typedef see_below type;
#else // defined(GENERATING_DOCUMENTATION)
  typedef typename detail::associated_executor_impl<T, Executor>::type type;
#endif // defined(GENERATING_DOCUMENTATION)

  /// If @c T has a nested type @c executor_type, returns
  /// <tt>t.get_executor()</tt>. Otherwise returns @c ex.
  static type get(const T& t,
      const Executor& ex = Executor()) NET_TS_NOEXCEPT
  {
    return detail::associated_executor_impl<T, Executor>::get(t, ex);
  }
};

/// Helper function to obtain an object's associated executor.
/**
 * @returns <tt>associated_executor<T>::get(t)</tt>
 */
template <typename T>
inline typename associated_executor<T>::type
get_associated_executor(const T& t) NET_TS_NOEXCEPT
{
  return associated_executor<T>::get(t);
}

/// Helper function to obtain an object's associated executor.
/**
 * @returns <tt>associated_executor<T, Executor>::get(t, ex)</tt>
 */
template <typename T, typename Executor>
inline typename associated_executor<T, Executor>::type
get_associated_executor(const T& t, const Executor& ex,
    typename enable_if<is_executor<
      Executor>::value>::type* = 0) NET_TS_NOEXCEPT
{
  return associated_executor<T, Executor>::get(t, ex);
}

/// Helper function to obtain an object's associated executor.
/**
 * @returns <tt>associated_executor<T, typename
 * ExecutionContext::executor_type>::get(t, ctx.get_executor())</tt>
 */
template <typename T, typename ExecutionContext>
inline typename associated_executor<T,
  typename ExecutionContext::executor_type>::type
get_associated_executor(const T& t, ExecutionContext& ctx,
    typename enable_if<is_convertible<ExecutionContext&,
      execution_context&>::value>::type* = 0) NET_TS_NOEXCEPT
{
  return associated_executor<T,
    typename ExecutionContext::executor_type>::get(t, ctx.get_executor());
}

#if defined(NET_TS_HAS_ALIAS_TEMPLATES)

template <typename T, typename Executor = system_executor>
using associated_executor_t = typename associated_executor<T, Executor>::type;

#endif // defined(NET_TS_HAS_ALIAS_TEMPLATES)

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_ASSOCIATED_EXECUTOR_HPP
