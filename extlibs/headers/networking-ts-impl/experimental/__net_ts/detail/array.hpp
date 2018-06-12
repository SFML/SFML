//
// detail/array.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_ARRAY_HPP
#define NET_TS_DETAIL_ARRAY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_STD_ARRAY)
# include <array>
#else // defined(NET_TS_HAS_STD_ARRAY)
# include <boost/array.hpp>
#endif // defined(NET_TS_HAS_STD_ARRAY)

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if defined(NET_TS_HAS_STD_ARRAY)
using std::array;
#else // defined(NET_TS_HAS_STD_ARRAY)
using boost::array;
#endif // defined(NET_TS_HAS_STD_ARRAY)

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_ARRAY_HPP
