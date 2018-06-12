//
// detail/cstdint.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_CSTDINT_HPP
#define NET_TS_DETAIL_CSTDINT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_CSTDINT)
# include <cstdint>
#else // defined(NET_TS_HAS_CSTDINT)
# include <boost/cstdint.hpp>
#endif // defined(NET_TS_HAS_CSTDINT)

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

#if defined(NET_TS_HAS_CSTDINT)
using std::int16_t;
using std::int_least16_t;
using std::uint16_t;
using std::uint_least16_t;
using std::int32_t;
using std::int_least32_t;
using std::uint32_t;
using std::uint_least32_t;
using std::int64_t;
using std::int_least64_t;
using std::uint64_t;
using std::uint_least64_t;
using std::uintmax_t;
#else // defined(NET_TS_HAS_CSTDINT)
using boost::int16_t;
using boost::int_least16_t;
using boost::uint16_t;
using boost::uint_least16_t;
using boost::int32_t;
using boost::int_least32_t;
using boost::uint32_t;
using boost::uint_least32_t;
using boost::int64_t;
using boost::int_least64_t;
using boost::uint64_t;
using boost::uint_least64_t;
using boost::uintmax_t;
#endif // defined(NET_TS_HAS_CSTDINT)

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_CSTDINT_HPP
