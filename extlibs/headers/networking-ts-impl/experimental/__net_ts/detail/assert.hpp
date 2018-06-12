//
// detail/assert.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_ASSERT_HPP
#define NET_TS_DETAIL_ASSERT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_BOOST_ASSERT)
# include <boost/assert.hpp>
#else // defined(NET_TS_HAS_BOOST_ASSERT)
# include <cassert>
#endif // defined(NET_TS_HAS_BOOST_ASSERT)

#if defined(NET_TS_HAS_BOOST_ASSERT)
# define NET_TS_ASSERT(expr) BOOST_ASSERT(expr)
#else // defined(NET_TS_HAS_BOOST_ASSERT)
# define NET_TS_ASSERT(expr) assert(expr)
#endif // defined(NET_TS_HAS_BOOST_ASSERT)

#endif // NET_TS_DETAIL_ASSERT_HPP
