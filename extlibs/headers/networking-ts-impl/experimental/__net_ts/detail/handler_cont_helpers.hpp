//
// detail/handler_cont_helpers.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_HANDLER_CONT_HELPERS_HPP
#define NET_TS_DETAIL_HANDLER_CONT_HELPERS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/handler_continuation_hook.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

// Calls to networking_ts_handler_is_continuation must be made from a namespace that
// does not contain overloads of this function. This namespace is defined here
// for that purpose.
namespace networking_ts_handler_cont_helpers {

template <typename Context>
inline bool is_continuation(Context& context)
{
#if !defined(NET_TS_HAS_HANDLER_HOOKS)
  return false;
#else
  using std::experimental::net::v1::networking_ts_handler_is_continuation;
  return networking_ts_handler_is_continuation(
      std::experimental::net::v1::detail::addressof(context));
#endif
}

} // namespace networking_ts_handler_cont_helpers

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_HANDLER_CONT_HELPERS_HPP
