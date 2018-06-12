//
// detail/reactor_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_REACTOR_OP_HPP
#define NET_TS_DETAIL_REACTOR_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/operation.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class reactor_op
  : public operation
{
public:
  // The error code to be passed to the completion handler.
  std::error_code ec_;

  // The number of bytes transferred, to be passed to the completion handler.
  std::size_t bytes_transferred_;

  // Status returned by perform function. May be used to decide whether it is
  // worth performing more operations on the descriptor immediately.
  enum status { not_done, done, done_and_exhausted };

  // Perform the operation. Returns true if it is finished.
  status perform()
  {
    return perform_func_(this);
  }

protected:
  typedef status (*perform_func_type)(reactor_op*);

  reactor_op(perform_func_type perform_func, func_type complete_func)
    : operation(complete_func),
      bytes_transferred_(0),
      perform_func_(perform_func)
  {
  }

private:
  perform_func_type perform_func_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_REACTOR_OP_HPP
