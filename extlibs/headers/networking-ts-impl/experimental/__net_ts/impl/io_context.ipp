//
// impl/io_context.ipp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IMPL_IO_CONTEXT_IPP
#define NET_TS_IMPL_IO_CONTEXT_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/io_context.hpp>
#include <experimental/__net_ts/detail/concurrency_hint.hpp>
#include <experimental/__net_ts/detail/limits.hpp>
#include <experimental/__net_ts/detail/scoped_ptr.hpp>
#include <experimental/__net_ts/detail/service_registry.hpp>
#include <experimental/__net_ts/detail/throw_error.hpp>

#if defined(NET_TS_HAS_IOCP)
# include <experimental/__net_ts/detail/win_iocp_io_context.hpp>
#else
# include <experimental/__net_ts/detail/scheduler.hpp>
#endif

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

io_context::io_context()
  : impl_(add_impl(new impl_type(*this, NET_TS_CONCURRENCY_HINT_DEFAULT)))
{
}

io_context::io_context(int concurrency_hint)
  : impl_(add_impl(new impl_type(*this, concurrency_hint == 1
          ? NET_TS_CONCURRENCY_HINT_1 : concurrency_hint)))
{
}

io_context::impl_type& io_context::add_impl(io_context::impl_type* impl)
{
  std::experimental::net::v1::detail::scoped_ptr<impl_type> scoped_impl(impl);
  std::experimental::net::v1::add_service<impl_type>(*this, scoped_impl.get());
  return *scoped_impl.release();
}

io_context::~io_context()
{
}

io_context::count_type io_context::run()
{
  std::error_code ec;
  count_type s = impl_.run(ec);
  std::experimental::net::v1::detail::throw_error(ec);
  return s;
}

io_context::count_type io_context::run_one()
{
  std::error_code ec;
  count_type s = impl_.run_one(ec);
  std::experimental::net::v1::detail::throw_error(ec);
  return s;
}

io_context::count_type io_context::poll()
{
  std::error_code ec;
  count_type s = impl_.poll(ec);
  std::experimental::net::v1::detail::throw_error(ec);
  return s;
}

io_context::count_type io_context::poll_one()
{
  std::error_code ec;
  count_type s = impl_.poll_one(ec);
  std::experimental::net::v1::detail::throw_error(ec);
  return s;
}

void io_context::stop()
{
  impl_.stop();
}

bool io_context::stopped() const
{
  return impl_.stopped();
}

void io_context::restart()
{
  impl_.restart();
}

io_context::service::service(std::experimental::net::v1::io_context& owner)
  : execution_context::service(owner)
{
}

io_context::service::~service()
{
}

void io_context::service::shutdown()
{
}

void io_context::service::notify_fork(io_context::fork_event ev)
{
  (void)ev;
}

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IMPL_IO_CONTEXT_IPP
