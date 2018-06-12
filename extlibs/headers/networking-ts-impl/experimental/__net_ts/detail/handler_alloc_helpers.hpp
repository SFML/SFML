//
// detail/handler_alloc_helpers.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_HANDLER_ALLOC_HELPERS_HPP
#define NET_TS_DETAIL_HANDLER_ALLOC_HELPERS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/detail/noncopyable.hpp>
#include <experimental/__net_ts/detail/recycling_allocator.hpp>
#include <experimental/__net_ts/associated_allocator.hpp>
#include <experimental/__net_ts/handler_alloc_hook.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

// Calls to networking_ts_handler_allocate and networking_ts_handler_deallocate must be made from
// a namespace that does not contain any overloads of these functions. The
// networking_ts_handler_alloc_helpers namespace is defined here for that purpose.
namespace networking_ts_handler_alloc_helpers {

template <typename Handler>
inline void* allocate(std::size_t s, Handler& h)
{
#if !defined(NET_TS_HAS_HANDLER_HOOKS)
  return ::operator new(s);
#else
  using std::experimental::net::v1::networking_ts_handler_allocate;
  return networking_ts_handler_allocate(s, std::experimental::net::v1::detail::addressof(h));
#endif
}

template <typename Handler>
inline void deallocate(void* p, std::size_t s, Handler& h)
{
#if !defined(NET_TS_HAS_HANDLER_HOOKS)
  ::operator delete(p);
#else
  using std::experimental::net::v1::networking_ts_handler_deallocate;
  networking_ts_handler_deallocate(p, s, std::experimental::net::v1::detail::addressof(h));
#endif
}

} // namespace networking_ts_handler_alloc_helpers

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename Handler, typename T>
class hook_allocator
{
public:
  typedef T value_type;

  template <typename U>
  struct rebind
  {
    typedef hook_allocator<Handler, U> other;
  };

  explicit hook_allocator(Handler& h)
    : handler_(h)
  {
  }

  template <typename U>
  hook_allocator(const hook_allocator<Handler, U>& a)
    : handler_(a.handler_)
  {
  }

  T* allocate(std::size_t n)
  {
    return static_cast<T*>(
        networking_ts_handler_alloc_helpers::allocate(sizeof(T) * n, handler_));
  }

  void deallocate(T* p, std::size_t n)
  {
    networking_ts_handler_alloc_helpers::deallocate(p, sizeof(T) * n, handler_);
  }

//private:
  Handler& handler_;
};

template <typename Handler>
class hook_allocator<Handler, void>
{
public:
  typedef void value_type;

  template <typename U>
  struct rebind
  {
    typedef hook_allocator<Handler, U> other;
  };

  explicit hook_allocator(Handler& h)
    : handler_(h)
  {
  }

  template <typename U>
  hook_allocator(const hook_allocator<Handler, U>& a)
    : handler_(a.handler_)
  {
  }

//private:
  Handler& handler_;
};

template <typename Handler, typename Allocator>
struct get_hook_allocator
{
  typedef Allocator type;

  static type get(Handler&, const Allocator& a)
  {
    return a;
  }
};

template <typename Handler, typename T>
struct get_hook_allocator<Handler, std::allocator<T> >
{
  typedef hook_allocator<Handler, T> type;

  static type get(Handler& handler, const std::allocator<T>&)
  {
    return type(handler);
  }
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#define NET_TS_DEFINE_HANDLER_PTR(op) \
  struct ptr \
  { \
    Handler* h; \
    op* v; \
    op* p; \
    ~ptr() \
    { \
      reset(); \
    } \
    static op* allocate(Handler& handler) \
    { \
      typedef typename ::std::experimental::net::v1::associated_allocator< \
        Handler>::type associated_allocator_type; \
      typedef typename ::std::experimental::net::v1::detail::get_hook_allocator< \
        Handler, associated_allocator_type>::type hook_allocator_type; \
      NET_TS_REBIND_ALLOC(hook_allocator_type, op) a( \
            ::std::experimental::net::v1::detail::get_hook_allocator< \
              Handler, associated_allocator_type>::get( \
                handler, ::std::experimental::net::v1::get_associated_allocator(handler))); \
      return a.allocate(1); \
    } \
    void reset() \
    { \
      if (p) \
      { \
        p->~op(); \
        p = 0; \
      } \
      if (v) \
      { \
        typedef typename ::std::experimental::net::v1::associated_allocator< \
          Handler>::type associated_allocator_type; \
        typedef typename ::std::experimental::net::v1::detail::get_hook_allocator< \
          Handler, associated_allocator_type>::type hook_allocator_type; \
        NET_TS_REBIND_ALLOC(hook_allocator_type, op) a( \
              ::std::experimental::net::v1::detail::get_hook_allocator< \
                Handler, associated_allocator_type>::get( \
                  *h, ::std::experimental::net::v1::get_associated_allocator(*h))); \
        a.deallocate(static_cast<op*>(v), 1); \
        v = 0; \
      } \
    } \
  } \
  /**/

#define NET_TS_DEFINE_HANDLER_ALLOCATOR_PTR(op) \
  struct ptr \
  { \
    const Alloc* a; \
    void* v; \
    op* p; \
    ~ptr() \
    { \
      reset(); \
    } \
    static op* allocate(const Alloc& a) \
    { \
      typedef typename ::std::experimental::net::v1::detail::get_recycling_allocator< \
        Alloc>::type recycling_allocator_type; \
      NET_TS_REBIND_ALLOC(recycling_allocator_type, op) a1( \
            ::std::experimental::net::v1::detail::get_recycling_allocator<Alloc>::get(a)); \
      return a1.allocate(1); \
    } \
    void reset() \
    { \
      if (p) \
      { \
        p->~op(); \
        p = 0; \
      } \
      if (v) \
      { \
        typedef typename ::std::experimental::net::v1::detail::get_recycling_allocator< \
          Alloc>::type recycling_allocator_type; \
        NET_TS_REBIND_ALLOC(recycling_allocator_type, op) a1( \
              ::std::experimental::net::v1::detail::get_recycling_allocator<Alloc>::get(*a)); \
        a1.deallocate(static_cast<op*>(v), 1); \
        v = 0; \
      } \
    } \
  } \
  /**/

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_DETAIL_HANDLER_ALLOC_HELPERS_HPP
