//
// detail/posix_fd_set_adapter.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_POSIX_FD_SET_ADAPTER_HPP
#define NET_TS_DETAIL_POSIX_FD_SET_ADAPTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_WINDOWS) \
  && !defined(__CYGWIN__) \
  && !defined(NET_TS_WINDOWS_RUNTIME)

#include <cstring>
#include <experimental/__net_ts/detail/noncopyable.hpp>
#include <experimental/__net_ts/detail/reactor_op_queue.hpp>
#include <experimental/__net_ts/detail/socket_types.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

// Adapts the FD_SET type to meet the Descriptor_Set concept's requirements.
class posix_fd_set_adapter : noncopyable
{
public:
  posix_fd_set_adapter()
    : max_descriptor_(invalid_socket)
  {
    using namespace std; // Needed for memset on Solaris.
    FD_ZERO(&fd_set_);
  }

  void reset()
  {
    using namespace std; // Needed for memset on Solaris.
    FD_ZERO(&fd_set_);
  }

  bool set(socket_type descriptor)
  {
    if (descriptor < (socket_type)FD_SETSIZE)
    {
      if (max_descriptor_ == invalid_socket || descriptor > max_descriptor_)
        max_descriptor_ = descriptor;
      FD_SET(descriptor, &fd_set_);
      return true;
    }
    return false;
  }

  void set(reactor_op_queue<socket_type>& operations, op_queue<operation>& ops)
  {
    reactor_op_queue<socket_type>::iterator i = operations.begin();
    while (i != operations.end())
    {
      reactor_op_queue<socket_type>::iterator op_iter = i++;
      if (!set(op_iter->first))
      {
        std::error_code ec(error::fd_set_failure);
        operations.cancel_operations(op_iter, ops, ec);
      }
    }
  }

  bool is_set(socket_type descriptor) const
  {
    return FD_ISSET(descriptor, &fd_set_) != 0;
  }

  operator fd_set*()
  {
    return &fd_set_;
  }

  socket_type max_descriptor() const
  {
    return max_descriptor_;
  }

  void perform(reactor_op_queue<socket_type>& operations,
      op_queue<operation>& ops) const
  {
    reactor_op_queue<socket_type>::iterator i = operations.begin();
    while (i != operations.end())
    {
      reactor_op_queue<socket_type>::iterator op_iter = i++;
      if (is_set(op_iter->first))
        operations.perform_operations(op_iter, ops);
    }
  }

private:
  mutable fd_set fd_set_;
  socket_type max_descriptor_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // !defined(NET_TS_WINDOWS)
       // && !defined(__CYGWIN__)
       // && !defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_POSIX_FD_SET_ADAPTER_HPP
