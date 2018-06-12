//
// ts/netfwd.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_TS_NETFWD_HPP
#define NET_TS_TS_NETFWD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_CHRONO)
# include <experimental/__net_ts/detail/chrono.hpp>
#endif // defined(NET_TS_HAS_CHRONO)

#if defined(NET_TS_HAS_BOOST_DATE_TIME)
# include <experimental/__net_ts/detail/date_time_fwd.hpp>
#endif // defined(NET_TS_HAS_BOOST_DATE_TIME)

#if !defined(GENERATING_DOCUMENTATION)

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

class execution_context;

template <typename T, typename Executor>
class executor_binder;

template <typename Executor>
class executor_work_guard;

class system_executor;

class executor;

template <typename Executor>
class strand;

class io_context;

template <typename Clock>
struct wait_traits;

#if defined(NET_TS_HAS_BOOST_DATE_TIME)

template <typename Time>
struct time_traits;

#endif // defined(NET_TS_HAS_BOOST_DATE_TIME)

#if !defined(NET_TS_BASIC_WAITABLE_TIMER_FWD_DECL)
#define NET_TS_BASIC_WAITABLE_TIMER_FWD_DECL

template <typename Clock,
    typename WaitTraits = std::experimental::net::v1::wait_traits<Clock>
    NET_TS_SVC_TPARAM_DEF2(= waitable_timer_service<Clock, WaitTraits>)>
class basic_waitable_timer;

#endif // !defined(NET_TS_BASIC_WAITABLE_TIMER_FWD_DECL)

#if defined(NET_TS_HAS_CHRONO)

typedef basic_waitable_timer<chrono::system_clock> system_timer;

typedef basic_waitable_timer<chrono::steady_clock> steady_timer;

typedef basic_waitable_timer<chrono::high_resolution_clock>
  high_resolution_timer;

#endif // defined(NET_TS_HAS_CHRONO)

template <class Protocol NET_TS_SVC_TPARAM>
class basic_socket;

template <typename Protocol NET_TS_SVC_TPARAM>
class basic_datagram_socket;

template <typename Protocol NET_TS_SVC_TPARAM>
class basic_stream_socket;

template <typename Protocol NET_TS_SVC_TPARAM>
class basic_socket_acceptor;

#if !defined(NET_TS_BASIC_SOCKET_STREAMBUF_FWD_DECL)
#define NET_TS_BASIC_SOCKET_STREAMBUF_FWD_DECL

// Forward declaration with defaulted arguments.
template <typename Protocol
    NET_TS_SVC_TPARAM_DEF1(= stream_socket_service<Protocol>),
#if defined(NET_TS_HAS_BOOST_DATE_TIME) \
  || defined(GENERATING_DOCUMENTATION)
    typename Clock = boost::posix_time::ptime,
    typename WaitTraits = time_traits<Clock>
    NET_TS_SVC_TPARAM1_DEF2(= deadline_timer_service<Clock, WaitTraits>)>
#else
    typename Clock = chrono::steady_clock,
    typename WaitTraits = wait_traits<Clock>
    NET_TS_SVC_TPARAM1_DEF1(= steady_timer::service_type)>
#endif
class basic_socket_streambuf;

#endif // !defined(NET_TS_BASIC_SOCKET_STREAMBUF_FWD_DECL)

#if !defined(NET_TS_BASIC_SOCKET_IOSTREAM_FWD_DECL)
#define NET_TS_BASIC_SOCKET_IOSTREAM_FWD_DECL

// Forward declaration with defaulted arguments.
template <typename Protocol
    NET_TS_SVC_TPARAM_DEF1(= stream_socket_service<Protocol>),
#if defined(NET_TS_HAS_BOOST_DATE_TIME) \
  || defined(GENERATING_DOCUMENTATION)
    typename Clock = boost::posix_time::ptime,
    typename WaitTraits = time_traits<Clock>
    NET_TS_SVC_TPARAM1_DEF2(= deadline_timer_service<Clock, WaitTraits>)>
#else
    typename Clock = chrono::steady_clock,
    typename WaitTraits = wait_traits<Clock>
    NET_TS_SVC_TPARAM1_DEF1(= steady_timer::service_type)>
#endif
class basic_socket_iostream;

#endif // !defined(NET_TS_BASIC_SOCKET_IOSTREAM_FWD_DECL)

namespace ip {

class address;

class address_v4;

class address_v6;

template <typename Address>
class basic_address_iterator;

typedef basic_address_iterator<address_v4> address_v4_iterator;

typedef basic_address_iterator<address_v6> address_v6_iterator;

template <typename Address>
class basic_address_range;

typedef basic_address_range<address_v4> address_v4_range;

typedef basic_address_range<address_v6> address_v6_range;

class network_v4;

class network_v6;

template <typename InternetProtocol>
class basic_endpoint;

template <typename InternetProtocol>
class basic_resolver_entry;

template <typename InternetProtocol>
class basic_resolver_results;

template <typename InternetProtocol NET_TS_SVC_TPARAM>
class basic_resolver;

class tcp;

class udp;

} // namespace ip
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // !defined(GENERATING_DOCUMENTATION)

#endif // NET_TS_TS_NETFWD_HPP
