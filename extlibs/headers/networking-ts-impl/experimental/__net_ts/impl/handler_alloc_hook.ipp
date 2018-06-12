//
// impl/handler_alloc_hook.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IMPL_HANDLER_ALLOC_HOOK_IPP
#define NET_TS_IMPL_HANDLER_ALLOC_HOOK_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/detail/thread_context.hpp>
#include <experimental/__net_ts/detail/thread_info_base.hpp>
#include <experimental/__net_ts/handler_alloc_hook.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

void* networking_ts_handler_allocate(std::size_t size, ...)
{
#if !defined(NET_TS_DISABLE_SMALL_BLOCK_RECYCLING)
  return detail::thread_info_base::allocate(
      detail::thread_context::thread_call_stack::top(), size);
#else // !defined(NET_TS_DISABLE_SMALL_BLOCK_RECYCLING)
  return ::operator new(size);
#endif // !defined(NET_TS_DISABLE_SMALL_BLOCK_RECYCLING)
}

void networking_ts_handler_deallocate(void* pointer, std::size_t size, ...)
{
#if !defined(NET_TS_DISABLE_SMALL_BLOCK_RECYCLING)
  detail::thread_info_base::deallocate(
      detail::thread_context::thread_call_stack::top(), pointer, size);
#else // !defined(NET_TS_DISABLE_SMALL_BLOCK_RECYCLING)
  (void)size;
  ::operator delete(pointer);
#endif // !defined(NET_TS_DISABLE_SMALL_BLOCK_RECYCLING)
}

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IMPL_HANDLER_ALLOC_HOOK_IPP
