//
// async_result.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_ASYNC_RESULT_HPP
#define NET_TS_ASYNC_RESULT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/type_traits.hpp>
#include <experimental/__net_ts/handler_type.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

/// An interface for customising the behaviour of an initiating function.
/**
 * The async_result traits class is used for determining:
 *
 * @li the concrete completion handler type to be called at the end of the
 * asynchronous operation;
 *
 * @li the initiating function return type; and
 *
 * @li how the return value of the initiating function is obtained.
 *
 * The trait allows the handler and return types to be determined at the point
 * where the specific completion handler signature is known.
 *
 * This template may be specialised for user-defined completion token types.
 * The primary template assumes that the CompletionToken is the completion
 * handler.
 */
template <typename CompletionToken, typename Signature>
class async_result
{
public:
  /// The concrete completion handler type for the specific signature.
  typedef CompletionToken completion_handler_type;

  /// The return type of the initiating function.
  typedef void return_type;
  /// Construct an async result from a given handler.
  /**
   * When using a specalised async_result, the constructor has an opportunity
   * to initialise some state associated with the completion handler, which is
   * then returned from the initiating function.
   */
  explicit async_result(completion_handler_type& h)
    // No data members to initialise.
  {
    (void)h;
  }

  /// Obtain the value to be returned from the initiating function.
  return_type get()
  {
    // Nothing to do.
  }

private:
  async_result(const async_result&) NET_TS_DELETED;
  async_result& operator=(const async_result&) NET_TS_DELETED;

  // No data members.
};

/// Helper template to deduce the handler type from a CompletionToken, capture
/// a local copy of the handler, and then create an async_result for the
/// handler.
template <typename CompletionToken, typename Signature>
struct async_completion
{
  /// The real handler type to be used for the asynchronous operation.
  typedef typename std::experimental::net::v1::async_result<
    typename decay<CompletionToken>::type,
      Signature>::completion_handler_type completion_handler_type;

#if defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Constructor.
  /**
   * The constructor creates the concrete completion handler and makes the link
   * between the handler and the asynchronous result.
   */
  explicit async_completion(CompletionToken& token)
    : completion_handler(static_cast<typename conditional<
        is_same<CompletionToken, completion_handler_type>::value,
        completion_handler_type&, CompletionToken&&>::type>(token)),
      result(completion_handler)
  {
  }
#else // defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  explicit async_completion(typename decay<CompletionToken>::type& token)
    : completion_handler(token),
      result(completion_handler)
  {
  }

  explicit async_completion(const typename decay<CompletionToken>::type& token)
    : completion_handler(token),
      result(completion_handler)
  {
  }
#endif // defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// A copy of, or reference to, a real handler object.
#if defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  typename conditional<
    is_same<CompletionToken, completion_handler_type>::value,
    completion_handler_type&, completion_handler_type>::type completion_handler;
#else // defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  completion_handler_type completion_handler;
#endif // defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// The result of the asynchronous operation's initiating function.
  async_result<typename decay<CompletionToken>::type, Signature> result;
};

namespace detail {

template <typename CompletionToken, typename Signature>
struct async_result_helper
  : async_result<typename decay<CompletionToken>::type, Signature>
{
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(GENERATING_DOCUMENTATION)
# define NET_TS_INITFN_RESULT_TYPE(ct, sig) \
  void_or_deduced
#elif defined(_MSC_VER) && (_MSC_VER < 1500)
# define NET_TS_INITFN_RESULT_TYPE(ct, sig) \
  typename ::std::experimental::net::v1::detail::async_result_helper< \
    ct, sig>::return_type
#define NET_TS_HANDLER_TYPE(ct, sig) \
  typename ::std::experimental::net::v1::detail::async_result_helper< \
    ct, sig>::completion_handler_type
#else
# define NET_TS_INITFN_RESULT_TYPE(ct, sig) \
  typename ::std::experimental::net::v1::async_result< \
    typename ::std::experimental::net::v1::decay<ct>::type, sig>::return_type
#define NET_TS_HANDLER_TYPE(ct, sig) \
  typename ::std::experimental::net::v1::async_result< \
    typename ::std::experimental::net::v1::decay<ct>::type, sig>::completion_handler_type
#endif

#endif // NET_TS_ASYNC_RESULT_HPP
