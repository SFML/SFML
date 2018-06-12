//
// detail/type_traits.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_TYPE_TRAITS_HPP
#define NET_TS_DETAIL_TYPE_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_HAS_STD_TYPE_TRAITS)
# include <type_traits>
#else // defined(NET_TS_HAS_TYPE_TRAITS)
# include <boost/type_traits/add_const.hpp>
# include <boost/type_traits/conditional.hpp>
# include <boost/type_traits/decay.hpp>
# include <boost/type_traits/integral_constant.hpp>
# include <boost/type_traits/is_base_of.hpp>
# include <boost/type_traits/is_class.hpp>
# include <boost/type_traits/is_const.hpp>
# include <boost/type_traits/is_convertible.hpp>
# include <boost/type_traits/is_function.hpp>
# include <boost/type_traits/is_same.hpp>
# include <boost/type_traits/remove_pointer.hpp>
# include <boost/type_traits/remove_reference.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/utility/result_of.hpp>
#endif // defined(NET_TS_HAS_TYPE_TRAITS)

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

#if defined(NET_TS_HAS_STD_TYPE_TRAITS)
using std::add_const;
using std::conditional;
using std::decay;
using std::enable_if;
using std::false_type;
using std::integral_constant;
using std::is_base_of;
using std::is_class;
using std::is_const;
using std::is_convertible;
using std::is_function;
using std::is_same;
using std::remove_pointer;
using std::remove_reference;
using std::result_of;
using std::true_type;
#else // defined(NET_TS_HAS_STD_TYPE_TRAITS)
using boost::add_const;
template <bool Condition, typename Type = void>
struct enable_if : boost::enable_if_c<Condition, Type> {};
using boost::conditional;
using boost::decay;
using boost::false_type;
using boost::integral_constant;
using boost::is_base_of;
using boost::is_class;
using boost::is_const;
using boost::is_convertible;
using boost::is_function;
using boost::is_same;
using boost::remove_pointer;
using boost::remove_reference;
using boost::result_of;
using boost::true_type;
#endif // defined(NET_TS_HAS_STD_TYPE_TRAITS)

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#endif // NET_TS_DETAIL_TYPE_TRAITS_HPP
