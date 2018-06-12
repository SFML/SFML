//
// detail/impl/posix_thread.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_POSIX_THREAD_IPP
#define NET_TS_DETAIL_IMPL_POSIX_THREAD_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_PTHREADS)

#include <experimental/__net_ts/detail/posix_thread.hpp>
#include <experimental/__net_ts/detail/throw_error.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

posix_thread::~posix_thread()
{
  if (!joined_)
    ::pthread_detach(thread_);
}

void posix_thread::join()
{
  if (!joined_)
  {
    ::pthread_join(thread_, 0);
    joined_ = true;
  }
}

std::size_t posix_thread::hardware_concurrency()
{
#if defined(_SC_NPROCESSORS_ONLN)
  long result = sysconf(_SC_NPROCESSORS_ONLN);
  if (result > 0)
    return result;
#endif // defined(_SC_NPROCESSORS_ONLN)
  return 0;
}

void posix_thread::start_thread(func_base* arg)
{
  int error = ::pthread_create(&thread_, 0,
        networking_ts_detail_posix_thread_function, arg);
  if (error != 0)
  {
    delete arg;
    std::error_code ec(error,
        std::experimental::net::v1::error::get_system_category());
    std::experimental::net::v1::detail::throw_error(ec, "thread");
  }
}

void* networking_ts_detail_posix_thread_function(void* arg)
{
  posix_thread::auto_func_base_ptr func = {
      static_cast<posix_thread::func_base*>(arg) };
  func.ptr->run();
  return 0;
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_HAS_PTHREADS)

#endif // NET_TS_DETAIL_IMPL_POSIX_THREAD_IPP
