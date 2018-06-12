//
// detail/win_global.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_WIN_GLOBAL_HPP
#define NET_TS_DETAIL_WIN_GLOBAL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/static_mutex.hpp>
#include <experimental/__net_ts/detail/tss_ptr.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename T>
struct win_global_impl
{
  // Destructor automatically cleans up the global.
  ~win_global_impl()
  {
    delete ptr_;
  }

  static win_global_impl instance_;
  static static_mutex mutex_;
  static T* ptr_;
  static tss_ptr<T> tss_ptr_;
};

template <typename T>
win_global_impl<T> win_global_impl<T>::instance_ = { 0 };

template <typename T>
static_mutex win_global_impl<T>::mutex_ = NET_TS_STATIC_MUTEX_INIT;

template <typename T>
T* win_global_impl<T>::ptr_ = 0;

template <typename T>
tss_ptr<T> win_global_impl<T>::tss_ptr_;

template <typename T>
T& win_global()
{
  if (static_cast<T*>(win_global_impl<T>::tss_ptr_) == 0)
  {
    win_global_impl<T>::mutex_.init();
    static_mutex::scoped_lock lock(win_global_impl<T>::mutex_);
    win_global_impl<T>::ptr_ = new T;
    win_global_impl<T>::tss_ptr_ = win_global_impl<T>::ptr_;
  }

  return *win_global_impl<T>::tss_ptr_;
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_WIN_GLOBAL_HPP
