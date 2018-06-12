//
// detail/tss_ptr.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_TSS_PTR_HPP
#define NET_TS_DETAIL_TSS_PTR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_HAS_THREADS)
# include <experimental/__net_ts/detail/null_tss_ptr.hpp>
#elif defined(NET_TS_HAS_THREAD_KEYWORD_EXTENSION)
# include <experimental/__net_ts/detail/keyword_tss_ptr.hpp>
#elif defined(NET_TS_WINDOWS)
# include <experimental/__net_ts/detail/win_tss_ptr.hpp>
#elif defined(NET_TS_HAS_PTHREADS)
# include <experimental/__net_ts/detail/posix_tss_ptr.hpp>
#else
# error Only Windows and POSIX are supported!
#endif

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename T>
class tss_ptr
#if !defined(NET_TS_HAS_THREADS)
  : public null_tss_ptr<T>
#elif defined(NET_TS_HAS_THREAD_KEYWORD_EXTENSION)
  : public keyword_tss_ptr<T>
#elif defined(NET_TS_WINDOWS)
  : public win_tss_ptr<T>
#elif defined(NET_TS_HAS_PTHREADS)
  : public posix_tss_ptr<T>
#endif
{
public:
  void operator=(T* value)
  {
#if !defined(NET_TS_HAS_THREADS)
    null_tss_ptr<T>::operator=(value);
#elif defined(NET_TS_HAS_THREAD_KEYWORD_EXTENSION)
    keyword_tss_ptr<T>::operator=(value);
#elif defined(NET_TS_WINDOWS)
    win_tss_ptr<T>::operator=(value);
#elif defined(NET_TS_HAS_PTHREADS)
    posix_tss_ptr<T>::operator=(value);
#endif
  }
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_TSS_PTR_HPP
