//
// detail/impl/win_tss_ptr.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_WIN_TSS_PTR_IPP
#define NET_TS_DETAIL_IMPL_WIN_TSS_PTR_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS)

#include <experimental/__net_ts/detail/throw_error.hpp>
#include <experimental/__net_ts/detail/win_tss_ptr.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

DWORD win_tss_ptr_create()
{
#if defined(UNDER_CE)
  const DWORD out_of_indexes = 0xFFFFFFFF;
#else
  const DWORD out_of_indexes = TLS_OUT_OF_INDEXES;
#endif

  DWORD tss_key = ::TlsAlloc();
  if (tss_key == out_of_indexes)
  {
    DWORD last_error = ::GetLastError();
    std::error_code ec(last_error,
        std::experimental::net::v1::error::get_system_category());
    std::experimental::net::v1::detail::throw_error(ec, "tss");
  }
  return tss_key;
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_WINDOWS)

#endif // NET_TS_DETAIL_IMPL_WIN_TSS_PTR_IPP
