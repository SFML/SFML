//
// detail/impl/pipe_select_interrupter.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_PIPE_SELECT_INTERRUPTER_IPP
#define NET_TS_DETAIL_IMPL_PIPE_SELECT_INTERRUPTER_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_WINDOWS_RUNTIME)
#if !defined(NET_TS_WINDOWS)
#if !defined(__CYGWIN__)
#if !defined(__SYMBIAN32__)
#if !defined(NET_TS_HAS_EVENTFD)

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <experimental/__net_ts/detail/pipe_select_interrupter.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>
#include <experimental/__net_ts/detail/throw_error.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

pipe_select_interrupter::pipe_select_interrupter()
{
  open_descriptors();
}

void pipe_select_interrupter::open_descriptors()
{
  int pipe_fds[2];
  if (pipe(pipe_fds) == 0)
  {
    read_descriptor_ = pipe_fds[0];
    ::fcntl(read_descriptor_, F_SETFL, O_NONBLOCK);
    write_descriptor_ = pipe_fds[1];
    ::fcntl(write_descriptor_, F_SETFL, O_NONBLOCK);

#if defined(FD_CLOEXEC)
    ::fcntl(read_descriptor_, F_SETFD, FD_CLOEXEC);
    ::fcntl(write_descriptor_, F_SETFD, FD_CLOEXEC);
#endif // defined(FD_CLOEXEC)
  }
  else
  {
    std::error_code ec(errno,
        std::experimental::net::v1::error::get_system_category());
    std::experimental::net::v1::detail::throw_error(ec, "pipe_select_interrupter");
  }
}

pipe_select_interrupter::~pipe_select_interrupter()
{
  close_descriptors();
}

void pipe_select_interrupter::close_descriptors()
{
  if (read_descriptor_ != -1)
    ::close(read_descriptor_);
  if (write_descriptor_ != -1)
    ::close(write_descriptor_);
}

void pipe_select_interrupter::recreate()
{
  close_descriptors();

  write_descriptor_ = -1;
  read_descriptor_ = -1;

  open_descriptors();
}

void pipe_select_interrupter::interrupt()
{
  char byte = 0;
  signed_size_type result = ::write(write_descriptor_, &byte, 1);
  (void)result;
}

bool pipe_select_interrupter::reset()
{
  for (;;)
  {
    char data[1024];
    signed_size_type bytes_read = ::read(read_descriptor_, data, sizeof(data));
    if (bytes_read < 0 && errno == EINTR)
      continue;
    bool was_interrupted = (bytes_read > 0);
    while (bytes_read == sizeof(data))
      bytes_read = ::read(read_descriptor_, data, sizeof(data));
    return was_interrupted;
  }
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // !defined(NET_TS_HAS_EVENTFD)
#endif // !defined(__SYMBIAN32__)
#endif // !defined(__CYGWIN__)
#endif // !defined(NET_TS_WINDOWS)
#endif // !defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_IMPL_PIPE_SELECT_INTERRUPTER_IPP
