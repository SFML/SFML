//
// detail/string_view.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_STRING_VIEW_HPP
#define NET_TS_DETAIL_STRING_VIEW_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_STD_STRING_VIEW)

#if defined(NET_TS_HAS_STD_EXPERIMENTAL_STRING_VIEW)
# include <experimental/string_view>
#else // defined(NET_TS_HAS_EXPERIMENTAL_STRING_VIEW)
# include <string_view>
#endif // defined(NET_TS_HAS_EXPERIMENTAL_STRING_VIEW)

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

#if defined(NET_TS_HAS_STD_EXPERIMENTAL_STRING_VIEW)
using std::experimental::basic_string_view;
using std::experimental::string_view;
#else // defined(NET_TS_HAS_STD_EXPERIMENTAL_STRING_VIEW)
using std::basic_string_view;
using std::string_view;
#endif // defined(NET_TS_HAS_STD_EXPERIMENTAL_STRING_VIEW)

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

# define NET_TS_STRING_VIEW_PARAM std::experimental::net::v1::string_view
#else // defined(NET_TS_HAS_STD_STRING_VIEW)
# define NET_TS_STRING_VIEW_PARAM const std::string&
#endif // defined(NET_TS_HAS_STD_STRING_VIEW)

#endif // NET_TS_DETAIL_STRING_VIEW_HPP
