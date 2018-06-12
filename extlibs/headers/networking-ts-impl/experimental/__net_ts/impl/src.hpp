//
// impl/src.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IMPL_SRC_HPP
#define NET_TS_IMPL_SRC_HPP

#define NET_TS_SOURCE

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HEADER_ONLY)
# error Do not compile Asio library source with NET_TS_HEADER_ONLY defined
#endif

#include <experimental/__net_ts/impl/error.ipp>
#include <experimental/__net_ts/impl/execution_context.ipp>
#include <experimental/__net_ts/impl/executor.ipp>
#include <experimental/__net_ts/impl/handler_alloc_hook.ipp>
#include <experimental/__net_ts/impl/io_context.ipp>
#include <experimental/__net_ts/impl/system_context.ipp>
#include <experimental/__net_ts/impl/thread_pool.ipp>
#include <experimental/__net_ts/detail/impl/buffer_sequence_adapter.ipp>
#include <experimental/__net_ts/detail/impl/descriptor_ops.ipp>
#include <experimental/__net_ts/detail/impl/dev_poll_reactor.ipp>
#include <experimental/__net_ts/detail/impl/epoll_reactor.ipp>
#include <experimental/__net_ts/detail/impl/eventfd_select_interrupter.ipp>
#include <experimental/__net_ts/detail/impl/handler_tracking.ipp>
#include <experimental/__net_ts/detail/impl/kqueue_reactor.ipp>
#include <experimental/__net_ts/detail/impl/null_event.ipp>
#include <experimental/__net_ts/detail/impl/pipe_select_interrupter.ipp>
#include <experimental/__net_ts/detail/impl/posix_event.ipp>
#include <experimental/__net_ts/detail/impl/posix_mutex.ipp>
#include <experimental/__net_ts/detail/impl/posix_thread.ipp>
#include <experimental/__net_ts/detail/impl/posix_tss_ptr.ipp>
#include <experimental/__net_ts/detail/impl/reactive_descriptor_service.ipp>
#include <experimental/__net_ts/detail/impl/reactive_socket_service_base.ipp>
#include <experimental/__net_ts/detail/impl/resolver_service_base.ipp>
#include <experimental/__net_ts/detail/impl/scheduler.ipp>
#include <experimental/__net_ts/detail/impl/select_reactor.ipp>
#include <experimental/__net_ts/detail/impl/service_registry.ipp>
#include <experimental/__net_ts/detail/impl/socket_ops.ipp>
#include <experimental/__net_ts/detail/impl/socket_select_interrupter.ipp>
#include <experimental/__net_ts/detail/impl/strand_executor_service.ipp>
#include <experimental/__net_ts/detail/impl/strand_service.ipp>
#include <experimental/__net_ts/detail/impl/throw_error.ipp>
#include <experimental/__net_ts/detail/impl/timer_queue_ptime.ipp>
#include <experimental/__net_ts/detail/impl/timer_queue_set.ipp>
#include <experimental/__net_ts/detail/impl/win_iocp_handle_service.ipp>
#include <experimental/__net_ts/detail/impl/win_iocp_io_context.ipp>
#include <experimental/__net_ts/detail/impl/win_iocp_socket_service_base.ipp>
#include <experimental/__net_ts/detail/impl/win_event.ipp>
#include <experimental/__net_ts/detail/impl/win_mutex.ipp>
#include <experimental/__net_ts/detail/impl/win_object_handle_service.ipp>
#include <experimental/__net_ts/detail/impl/win_static_mutex.ipp>
#include <experimental/__net_ts/detail/impl/win_thread.ipp>
#include <experimental/__net_ts/detail/impl/win_tss_ptr.ipp>
#include <experimental/__net_ts/detail/impl/winrt_ssocket_service_base.ipp>
#include <experimental/__net_ts/detail/impl/winrt_timer_scheduler.ipp>
#include <experimental/__net_ts/detail/impl/winsock_init.ipp>
#include <experimental/__net_ts/ip/impl/address.ipp>
#include <experimental/__net_ts/ip/impl/address_v4.ipp>
#include <experimental/__net_ts/ip/impl/address_v6.ipp>
#include <experimental/__net_ts/ip/impl/host_name.ipp>
#include <experimental/__net_ts/ip/impl/network_v4.ipp>
#include <experimental/__net_ts/ip/impl/network_v6.ipp>
#include <experimental/__net_ts/ip/detail/impl/endpoint.ipp>

#endif // NET_TS_IMPL_SRC_HPP
