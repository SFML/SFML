//
// steady_timer.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_STEADY_TIMER_HPP
#define NET_TS_STEADY_TIMER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_CHRONO) || defined(GENERATING_DOCUMENTATION)

#include <experimental/__net_ts/basic_waitable_timer.hpp>
#include <experimental/__net_ts/detail/chrono.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

/// Typedef for a timer based on the steady clock.
/**
 * This typedef uses the C++11 @c &lt;chrono&gt; standard library facility, if
 * available. Otherwise, it may use the Boost.Chrono library. To explicitly
 * utilise Boost.Chrono, use the basic_waitable_timer template directly:
 * @code
 * typedef basic_waitable_timer<boost::chrono::steady_clock> timer;
 * @endcode
 */
typedef basic_waitable_timer<chrono::steady_clock> steady_timer;

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // defined(NET_TS_HAS_CHRONO) || defined(GENERATING_DOCUMENTATION)

#endif // NET_TS_STEADY_TIMER_HPP
