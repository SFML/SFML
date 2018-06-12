//
// detail/pipe_select_interrupter.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_PIPE_SELECT_INTERRUPTER_HPP
#define NET_TS_DETAIL_PIPE_SELECT_INTERRUPTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_WINDOWS)
#if !defined(NET_TS_WINDOWS_RUNTIME)
#if !defined(__CYGWIN__)
#if !defined(__SYMBIAN32__)
#if !defined(NET_TS_HAS_EVENTFD)

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class pipe_select_interrupter
{
public:
  // Constructor.
  NET_TS_DECL pipe_select_interrupter();

  // Destructor.
  NET_TS_DECL ~pipe_select_interrupter();

  // Recreate the interrupter's descriptors. Used after a fork.
  NET_TS_DECL void recreate();

  // Interrupt the select call.
  NET_TS_DECL void interrupt();

  // Reset the select interrupt. Returns true if the call was interrupted.
  NET_TS_DECL bool reset();

  // Get the read descriptor to be passed to select.
  int read_descriptor() const
  {
    return read_descriptor_;
  }

private:
  // Open the descriptors. Throws on error.
  NET_TS_DECL void open_descriptors();

  // Close the descriptors.
  NET_TS_DECL void close_descriptors();

  // The read end of a connection used to interrupt the select call. This file
  // descriptor is passed to select such that when it is time to stop, a single
  // byte will be written on the other end of the connection and this
  // descriptor will become readable.
  int read_descriptor_;

  // The write end of a connection used to interrupt the select call. A single
  // byte may be written to this to wake up the select which is waiting for the
  // other end to become readable.
  int write_descriptor_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/detail/impl/pipe_select_interrupter.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // !defined(NET_TS_HAS_EVENTFD)
#endif // !defined(__SYMBIAN32__)
#endif // !defined(__CYGWIN__)
#endif // !defined(NET_TS_WINDOWS_RUNTIME)
#endif // !defined(NET_TS_WINDOWS)

#endif // NET_TS_DETAIL_PIPE_SELECT_INTERRUPTER_HPP
