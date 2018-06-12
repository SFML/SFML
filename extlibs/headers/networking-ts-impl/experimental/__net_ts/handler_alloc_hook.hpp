//
// handler_alloc_hook.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_HANDLER_ALLOC_HOOK_HPP
#define NET_TS_HANDLER_ALLOC_HOOK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <cstddef>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

/// Default allocation function for handlers.
/**
 * Asynchronous operations may need to allocate temporary objects. Since
 * asynchronous operations have a handler function object, these temporary
 * objects can be said to be associated with the handler.
 *
 * Implement networking_ts_handler_allocate and networking_ts_handler_deallocate for your own
 * handlers to provide custom allocation for these temporary objects.
 *
 * The default implementation of these allocation hooks uses <tt>::operator
 * new</tt> and <tt>::operator delete</tt>.
 *
 * @note All temporary objects associated with a handler will be deallocated
 * before the upcall to the handler is performed. This allows the same memory to
 * be reused for a subsequent asynchronous operation initiated by the handler.
 *
 * @par Example
 * @code
 * class my_handler;
 *
 * void* networking_ts_handler_allocate(std::size_t size, my_handler* context)
 * {
 *   return ::operator new(size);
 * }
 *
 * void networking_ts_handler_deallocate(void* pointer, std::size_t size,
 *     my_handler* context)
 * {
 *   ::operator delete(pointer);
 * }
 * @endcode
 */
NET_TS_DECL void* networking_ts_handler_allocate(
    std::size_t size, ...);

/// Default deallocation function for handlers.
/**
 * Implement networking_ts_handler_allocate and networking_ts_handler_deallocate for your own
 * handlers to provide custom allocation for the associated temporary objects.
 *
 * The default implementation of these allocation hooks uses <tt>::operator
 * new</tt> and <tt>::operator delete</tt>.
 *
 * @sa networking_ts_handler_allocate.
 */
NET_TS_DECL void networking_ts_handler_deallocate(
    void* pointer, std::size_t size, ...);

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if defined(NET_TS_HEADER_ONLY)
# include <experimental/__net_ts/impl/handler_alloc_hook.ipp>
#endif // defined(NET_TS_HEADER_ONLY)

#endif // NET_TS_HANDLER_ALLOC_HOOK_HPP
