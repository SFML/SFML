//
// detail/impl/throw_error.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_THROW_ERROR_IPP
#define NET_TS_DETAIL_IMPL_THROW_ERROR_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/throw_error.hpp>
#include <experimental/__net_ts/detail/throw_exception.hpp>
#include <system_error>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

void do_throw_error(const std::error_code& err)
{
  std::system_error e(err);
  std::experimental::net::v1::detail::throw_exception(e);
}

void do_throw_error(const std::error_code& err, const char* location)
{
  // boostify: non-boost code starts here
#if defined(NET_TS_MSVC) && defined(NET_TS_HAS_STD_SYSTEM_ERROR)
  // Microsoft's implementation of std::system_error is non-conformant in that
  // it ignores the error code's message when a "what" string is supplied. We'll
  // work around this by explicitly formatting the "what" string.
  std::string what_msg = location;
  what_msg += ": ";
  what_msg += err.message();
  std::system_error e(err, what_msg);
  std::experimental::net::v1::detail::throw_exception(e);
#else // defined(NET_TS_MSVC) && defined(NET_TS_HAS_STD_SYSTEM_ERROR)
  // boostify: non-boost code ends here
  std::system_error e(err, location);
  std::experimental::net::v1::detail::throw_exception(e);
  // boostify: non-boost code starts here
#endif // defined(NET_TS_MSVC) && defined(NET_TS_HAS_STD_SYSTEM_ERROR)
  // boostify: non-boost code ends here
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_IMPL_THROW_ERROR_IPP
