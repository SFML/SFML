//
// detail/winrt_resolve_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_WINRT_RESOLVE_OP_HPP
#define NET_TS_DETAIL_WINRT_RESOLVE_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS_RUNTIME)

#include <experimental/__net_ts/detail/bind_handler.hpp>
#include <experimental/__net_ts/detail/fenced_block.hpp>
#include <experimental/__net_ts/detail/handler_alloc_helpers.hpp>
#include <experimental/__net_ts/detail/handler_invoke_helpers.hpp>
#include <experimental/__net_ts/detail/memory.hpp>
#include <experimental/__net_ts/detail/winrt_async_op.hpp>
#include <experimental/__net_ts/ip/basic_resolver_results.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

template <typename Protocol, typename Handler>
class winrt_resolve_op :
  public winrt_async_op<
    Windows::Foundation::Collections::IVectorView<
      Windows::Networking::EndpointPair^>^>
{
public:
  NET_TS_DEFINE_HANDLER_PTR(winrt_resolve_op);

  typedef typename Protocol::endpoint endpoint_type;
  typedef std::experimental::net::v1::ip::basic_resolver_query<Protocol> query_type;
  typedef std::experimental::net::v1::ip::basic_resolver_results<Protocol> results_type;

  winrt_resolve_op(const query_type& query, Handler& handler)
    : winrt_async_op<
        Windows::Foundation::Collections::IVectorView<
          Windows::Networking::EndpointPair^>^>(
            &winrt_resolve_op::do_complete),
      query_(query),
      handler_(NET_TS_MOVE_CAST(Handler)(handler))
  {
    handler_work<Handler>::start(handler_);
  }

  static void do_complete(void* owner, operation* base,
      const std::error_code&, std::size_t)
  {
    // Take ownership of the operation object.
    winrt_resolve_op* o(static_cast<winrt_resolve_op*>(base));
    ptr p = { std::experimental::net::v1::detail::addressof(o->handler_), o, o };
    handler_work<Handler> w(o->handler_);

    NET_TS_HANDLER_COMPLETION((*o));

    results_type results = results_type();
    if (!o->ec_)
    {
      try
      {
        results = results_type::create(o->result_, o->query_.hints(),
            o->query_.host_name(), o->query_.service_name());
      }
      catch (Platform::Exception^ e)
      {
        o->ec_ = std::error_code(e->HResult,
            std::system_category());
      }
    }

    // Make a copy of the handler so that the memory can be deallocated before
    // the upcall is made. Even if we're not about to make an upcall, a
    // sub-object of the handler may be the true owner of the memory associated
    // with the handler. Consequently, a local copy of the handler is required
    // to ensure that any owning sub-object remains valid until after we have
    // deallocated the memory here.
    detail::binder2<Handler, std::error_code, results_type>
      handler(o->handler_, o->ec_, results);
    p.h = std::experimental::net::v1::detail::addressof(handler.handler_);
    p.reset();

    // Make the upcall if required.
    if (owner)
    {
      fenced_block b(fenced_block::half);
      NET_TS_HANDLER_INVOCATION_BEGIN((handler.arg1_, "..."));
      w.complete(handler, handler.handler_);
      NET_TS_HANDLER_INVOCATION_END;
    }
  }

private:
  query_type query_;
  Handler handler_;
};

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_WINRT_RESOLVE_OP_HPP
