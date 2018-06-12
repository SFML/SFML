//
// impl/execution_context.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IMPL_EXECUTION_CONTEXT_HPP
#define NET_TS_IMPL_EXECUTION_CONTEXT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/handler_type_requirements.hpp>
#include <experimental/__net_ts/detail/scoped_ptr.hpp>
#include <experimental/__net_ts/detail/service_registry.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

template <typename Service>
inline Service& use_service(execution_context& e)
{
  // Check that Service meets the necessary type requirements.
  (void)static_cast<execution_context::service*>(static_cast<Service*>(0));

  return e.service_registry_->template use_service<Service>();
}

#if !defined(GENERATING_DOCUMENTATION)
# if defined(NET_TS_HAS_VARIADIC_TEMPLATES)

template <typename Service, typename... Args>
Service& make_service(execution_context& e, NET_TS_MOVE_ARG(Args)... args)
{
  detail::scoped_ptr<Service> svc(
      new Service(e, NET_TS_MOVE_CAST(Args)(args)...));
  e.service_registry_->template add_service<Service>(svc.get());
  Service& result = *svc;
  svc.release();
  return result;
}

# else // defined(NET_TS_HAS_VARIADIC_TEMPLATES)

template <typename Service>
Service& make_service(execution_context& e)
{
  detail::scoped_ptr<Service> svc(new Service(e));
  e.service_registry_->template add_service<Service>(svc.get());
  Service& result = *svc;
  svc.release();
  return result;
}

#define NET_TS_PRIVATE_MAKE_SERVICE_DEF(n) \
  template <typename Service, NET_TS_VARIADIC_TPARAMS(n)> \
  Service& make_service(execution_context& e, \
      NET_TS_VARIADIC_MOVE_PARAMS(n)) \
  { \
    detail::scoped_ptr<Service> svc( \
        new Service(e, NET_TS_VARIADIC_MOVE_ARGS(n))); \
    e.service_registry_->template add_service<Service>(svc.get()); \
    Service& result = *svc; \
    svc.release(); \
    return result; \
  } \
  /**/
  NET_TS_VARIADIC_GENERATE(NET_TS_PRIVATE_MAKE_SERVICE_DEF)
#undef NET_TS_PRIVATE_MAKE_SERVICE_DEF

# endif // defined(NET_TS_HAS_VARIADIC_TEMPLATES)
#endif // !defined(GENERATING_DOCUMENTATION)

template <typename Service>
inline void add_service(execution_context& e, Service* svc)
{
  // Check that Service meets the necessary type requirements.
  (void)static_cast<execution_context::service*>(static_cast<Service*>(0));

  e.service_registry_->template add_service<Service>(svc);
}

template <typename Service>
inline bool has_service(execution_context& e)
{
  // Check that Service meets the necessary type requirements.
  (void)static_cast<execution_context::service*>(static_cast<Service*>(0));

  return e.service_registry_->template has_service<Service>();
}

inline execution_context& execution_context::service::context()
{
  return owner_;
}

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IMPL_EXECUTION_CONTEXT_HPP
