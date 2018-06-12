//
// basic_io_object.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_BASIC_IO_OBJECT_HPP
#define NET_TS_BASIC_IO_OBJECT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <experimental/__net_ts/io_context.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

#if defined(NET_TS_HAS_MOVE)
namespace detail
{
  // Type trait used to determine whether a service supports move.
  template <typename IoObjectService>
  class service_has_move
  {
  private:
    typedef IoObjectService service_type;
    typedef typename service_type::implementation_type implementation_type;

    template <typename T, typename U>
    static auto asio_service_has_move_eval(T* t, U* u)
      -> decltype(t->move_construct(*u, *u), char());
    static char (&asio_service_has_move_eval(...))[2];

  public:
    static const bool value =
      sizeof(asio_service_has_move_eval(
        static_cast<service_type*>(0),
        static_cast<implementation_type*>(0))) == 1;
  };
}
#endif // defined(NET_TS_HAS_MOVE)

/// Base class for all I/O objects.
/**
 * @note All I/O objects are non-copyable. However, when using C++0x, certain
 * I/O objects do support move construction and move assignment.
 */
#if !defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
template <typename IoObjectService>
#else
template <typename IoObjectService,
    bool Movable = detail::service_has_move<IoObjectService>::value>
#endif
class basic_io_object
{
public:
  /// The type of the service that will be used to provide I/O operations.
  typedef IoObjectService service_type;

  /// The underlying implementation type of I/O object.
  typedef typename service_type::implementation_type implementation_type;

  /// The type of the executor associated with the object.
  typedef std::experimental::net::v1::io_context::executor_type executor_type;

  /// Get the executor associated with the object.
  executor_type get_executor() NET_TS_NOEXCEPT
  {
    return service_.get_io_context().get_executor();
  }

protected:
  /// Construct a basic_io_object.
  /**
   * Performs:
   * @code get_service().construct(get_implementation()); @endcode
   */
  explicit basic_io_object(std::experimental::net::v1::io_context& io_context)
    : service_(std::experimental::net::v1::use_service<IoObjectService>(io_context))
  {
    service_.construct(implementation_);
  }

#if defined(GENERATING_DOCUMENTATION)
  /// Move-construct a basic_io_object.
  /**
   * Performs:
   * @code get_service().move_construct(
   *     get_implementation(), other.get_implementation()); @endcode
   *
   * @note Available only for services that support movability,
   */
  basic_io_object(basic_io_object&& other);

  /// Move-assign a basic_io_object.
  /**
   * Performs:
   * @code get_service().move_assign(get_implementation(),
   *     other.get_service(), other.get_implementation()); @endcode
   *
   * @note Available only for services that support movability,
   */
  basic_io_object& operator=(basic_io_object&& other);

  /// Perform a converting move-construction of a basic_io_object.
  template <typename IoObjectService1>
  basic_io_object(IoObjectService1& other_service,
      typename IoObjectService1::implementation_type& other_implementation);
#endif // defined(GENERATING_DOCUMENTATION)

  /// Protected destructor to prevent deletion through this type.
  /**
   * Performs:
   * @code get_service().destroy(get_implementation()); @endcode
   */
  ~basic_io_object()
  {
    service_.destroy(implementation_);
  }

  /// Get the service associated with the I/O object.
  service_type& get_service()
  {
    return service_;
  }

  /// Get the service associated with the I/O object.
  const service_type& get_service() const
  {
    return service_;
  }

  /// Get the underlying implementation of the I/O object.
  implementation_type& get_implementation()
  {
    return implementation_;
  }

  /// Get the underlying implementation of the I/O object.
  const implementation_type& get_implementation() const
  {
    return implementation_;
  }

private:
  basic_io_object(const basic_io_object&);
  basic_io_object& operator=(const basic_io_object&);

  // The service associated with the I/O object.
  service_type& service_;

  /// The underlying implementation of the I/O object.
  implementation_type implementation_;
};

#if defined(NET_TS_HAS_MOVE)
// Specialisation for movable objects.
template <typename IoObjectService>
class basic_io_object<IoObjectService, true>
{
public:
  typedef IoObjectService service_type;
  typedef typename service_type::implementation_type implementation_type;

  typedef std::experimental::net::v1::io_context::executor_type executor_type;

  executor_type get_executor() NET_TS_NOEXCEPT
  {
    return service_->get_io_context().get_executor();
  }

protected:
  explicit basic_io_object(std::experimental::net::v1::io_context& io_context)
    : service_(&std::experimental::net::v1::use_service<IoObjectService>(io_context))
  {
    service_->construct(implementation_);
  }

  basic_io_object(basic_io_object&& other)
    : service_(&other.get_service())
  {
    service_->move_construct(implementation_, other.implementation_);
  }

  template <typename IoObjectService1>
  basic_io_object(IoObjectService1& other_service,
      typename IoObjectService1::implementation_type& other_implementation)
    : service_(&std::experimental::net::v1::use_service<IoObjectService>(
          other_service.get_io_context()))
  {
    service_->converting_move_construct(implementation_,
        other_service, other_implementation);
  }

  ~basic_io_object()
  {
    service_->destroy(implementation_);
  }

  basic_io_object& operator=(basic_io_object&& other)
  {
    service_->move_assign(implementation_,
        *other.service_, other.implementation_);
    service_ = other.service_;
    return *this;
  }

  service_type& get_service()
  {
    return *service_;
  }

  const service_type& get_service() const
  {
    return *service_;
  }

  implementation_type& get_implementation()
  {
    return implementation_;
  }

  const implementation_type& get_implementation() const
  {
    return implementation_;
  }

private:
  basic_io_object(const basic_io_object&);
  void operator=(const basic_io_object&);

  IoObjectService* service_;
  implementation_type implementation_;
};
#endif // defined(NET_TS_HAS_MOVE)

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_BASIC_IO_OBJECT_HPP
