//
// detail/functional.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_FUNCTIONAL_HPP
#define NET_TS_DETAIL_FUNCTIONAL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#include <functional>

#if !defined(NET_TS_HAS_STD_FUNCTION)
# include <boost/function.hpp>
#endif // !defined(NET_TS_HAS_STD_FUNCTION)

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

#if defined(NET_TS_HAS_STD_FUNCTION)
using std::function;
#else // defined(NET_TS_HAS_STD_FUNCTION)
using boost::function;
#endif // defined(NET_TS_HAS_STD_FUNCTION)

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_FUNCTIONAL_HPP
