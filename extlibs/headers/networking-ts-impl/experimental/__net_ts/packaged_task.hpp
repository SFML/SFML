//
// packaged_task.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_PACKAGED_TASK_HPP
#define NET_TS_PACKAGED_TASK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_STD_FUTURE) \
  || defined(GENERATING_DOCUMENTATION)

#include <future>
#include <experimental/__net_ts/async_result.hpp>
#include <experimental/__net_ts/detail/type_traits.hpp>
#include <experimental/__net_ts/detail/variadic_templates.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

#if defined(NET_TS_HAS_VARIADIC_TEMPLATES) \
  || defined(GENERATING_DOCUMENTATION)

/// Partial specialisation of @c async_result for @c std::packaged_task.
template <typename Result, typename... Args, typename Signature>
class async_result<std::packaged_task<Result(Args...)>, Signature>
{
public:
  /// The packaged task is the concrete completion handler type.
  typedef std::packaged_task<Result(Args...)> completion_handler_type;

  /// The return type of the initiating function is the future obtained from
  /// the packaged task.
  typedef std::future<Result> return_type;

  /// The constructor extracts the future from the packaged task.
  explicit async_result(completion_handler_type& h)
    : future_(h.get_future())
  {
  }

  /// Returns the packaged task's future.
  return_type get()
  {
    return std::move(future_);
  }

private:
  return_type future_;
};

#else // defined(NET_TS_HAS_VARIADIC_TEMPLATES)
      //   || defined(GENERATING_DOCUMENTATION)

template <typename Result, typename Signature>
struct async_result<std::packaged_task<Result()>, Signature>
{
  typedef std::packaged_task<Result()> completion_handler_type;
  typedef std::future<Result> return_type;

  explicit async_result(completion_handler_type& h)
    : future_(h.get_future())
  {
  }

  return_type get()
  {
    return std::move(future_);
  }

private:
  return_type future_;
};

#define NET_TS_PRIVATE_ASYNC_RESULT_DEF(n) \
  template <typename Result, \
    NET_TS_VARIADIC_TPARAMS(n), typename Signature> \
  class async_result< \
    std::packaged_task<Result(NET_TS_VARIADIC_TARGS(n))>, Signature> \
  { \
  public: \
    typedef std::packaged_task< \
      Result(NET_TS_VARIADIC_TARGS(n))> \
        completion_handler_type; \
  \
    typedef std::future<Result> return_type; \
  \
    explicit async_result(completion_handler_type& h) \
      : future_(h.get_future()) \
    { \
    } \
  \
    return_type get() \
    { \
      return std::move(future_); \
    } \
  \
  private: \
    return_type future_; \
  }; \
  /**/
  NET_TS_VARIADIC_GENERATE(NET_TS_PRIVATE_ASYNC_RESULT_DEF)
#undef NET_TS_PRIVATE_ASYNC_RESULT_DEF

#endif // defined(NET_TS_HAS_VARIADIC_TEMPLATES)
       //   || defined(GENERATING_DOCUMENTATION)

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_HAS_STD_FUTURE)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // NET_TS_PACKAGED_TASK_HPP
