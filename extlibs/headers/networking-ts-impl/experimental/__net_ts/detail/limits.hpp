//
// detail/limits.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_LIMITS_HPP
#define NET_TS_DETAIL_LIMITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_BOOST_LIMITS)
# include <boost/limits.hpp>
#else // defined(NET_TS_HAS_BOOST_LIMITS)
# include <limits>
#endif // defined(NET_TS_HAS_BOOST_LIMITS)

#endif // NET_TS_DETAIL_LIMITS_HPP
