//
// basic_socket_iostream.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_BASIC_SOCKET_IOSTREAM_HPP
#define NET_TS_BASIC_SOCKET_IOSTREAM_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if !defined(NET_TS_NO_IOSTREAM)

#include <istream>
#include <ostream>
#include <experimental/__net_ts/basic_socket_streambuf.hpp>

#if !defined(NET_TS_HAS_VARIADIC_TEMPLATES)

# include <experimental/__net_ts/detail/variadic_templates.hpp>

// A macro that should expand to:
//   template <typename T1, ..., typename Tn>
//   explicit basic_socket_iostream(T1 x1, ..., Tn xn)
//     : std::basic_iostream<char>(
//         &this->detail::socket_iostream_base<
//           Protocol NET_TS_SVC_TARG, Clock,
//           WaitTraits NET_TS_SVC_TARG1>::streambuf_)
//   {
//     if (rdbuf()->connect(x1, ..., xn) == 0)
//       this->setstate(std::ios_base::failbit);
//   }
// This macro should only persist within this file.

# define NET_TS_PRIVATE_CTR_DEF(n) \
  template <NET_TS_VARIADIC_TPARAMS(n)> \
  explicit basic_socket_iostream(NET_TS_VARIADIC_BYVAL_PARAMS(n)) \
    : std::basic_iostream<char>( \
        &this->detail::socket_iostream_base< \
          Protocol NET_TS_SVC_TARG, Clock, \
          WaitTraits NET_TS_SVC_TARG1>::streambuf_) \
  { \
    this->setf(std::ios_base::unitbuf); \
    if (rdbuf()->connect(NET_TS_VARIADIC_BYVAL_ARGS(n)) == 0) \
      this->setstate(std::ios_base::failbit); \
  } \
  /**/

// A macro that should expand to:
//   template <typename T1, ..., typename Tn>
//   void connect(T1 x1, ..., Tn xn)
//   {
//     if (rdbuf()->connect(x1, ..., xn) == 0)
//       this->setstate(std::ios_base::failbit);
//   }
// This macro should only persist within this file.

# define NET_TS_PRIVATE_CONNECT_DEF(n) \
  template <NET_TS_VARIADIC_TPARAMS(n)> \
  void connect(NET_TS_VARIADIC_BYVAL_PARAMS(n)) \
  { \
    if (rdbuf()->connect(NET_TS_VARIADIC_BYVAL_ARGS(n)) == 0) \
      this->setstate(std::ios_base::failbit); \
  } \
  /**/

#endif // !defined(NET_TS_HAS_VARIADIC_TEMPLATES)

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

// A separate base class is used to ensure that the streambuf is initialised
// prior to the basic_socket_iostream's basic_iostream base class.
template <typename Protocol NET_TS_SVC_TPARAM,
    typename Clock, typename WaitTraits NET_TS_SVC_TPARAM1>
class socket_iostream_base
{
protected:
  socket_iostream_base()
  {
  }

#if defined(NET_TS_HAS_MOVE)
  socket_iostream_base(socket_iostream_base&& other)
    : streambuf_(std::move(other.streambuf_))
  {
  }

  socket_iostream_base(basic_stream_socket<Protocol> s)
    : streambuf_(std::move(s))
  {
  }

  socket_iostream_base& operator=(socket_iostream_base&& other)
  {
    streambuf_ = std::move(other.streambuf_);
    return *this;
  }
#endif // defined(NET_TS_HAS_MOVE)

  basic_socket_streambuf<Protocol NET_TS_SVC_TARG,
    Clock, WaitTraits NET_TS_SVC_TARG1> streambuf_;
};

} // namespace detail

#if !defined(NET_TS_BASIC_SOCKET_IOSTREAM_FWD_DECL)
#define NET_TS_BASIC_SOCKET_IOSTREAM_FWD_DECL

// Forward declaration with defaulted arguments.
template <typename Protocol
    NET_TS_SVC_TPARAM_DEF1(= stream_socket_service<Protocol>),
#if defined(NET_TS_HAS_BOOST_DATE_TIME)
    typename Clock = boost::posix_time::ptime,
    typename WaitTraits = time_traits<Clock>
    NET_TS_SVC_TPARAM1_DEF2(= deadline_timer_service<Clock, WaitTraits>)>
#else
    typename Clock = chrono::steady_clock,
    typename WaitTraits = wait_traits<Clock>
    NET_TS_SVC_TPARAM1_DEF1(= steady_timer::service_type)>
#endif
class basic_socket_iostream;

#endif // !defined(NET_TS_BASIC_SOCKET_IOSTREAM_FWD_DECL)

/// Iostream interface for a socket.
#if defined(GENERATING_DOCUMENTATION)
template <typename Protocol,
    typename Clock = chrono::steady_clock,
    typename WaitTraits = wait_traits<Clock> >
#else // defined(GENERATING_DOCUMENTATION)
template <typename Protocol NET_TS_SVC_TPARAM,
    typename Clock, typename WaitTraits NET_TS_SVC_TPARAM1>
#endif // defined(GENERATING_DOCUMENTATION)
class basic_socket_iostream
  : private detail::socket_iostream_base<Protocol
        NET_TS_SVC_TARG, Clock, WaitTraits NET_TS_SVC_TARG1>,
    public std::basic_iostream<char>
{
private:
  // These typedefs are intended keep this class's implementation independent
  // of whether it's using Boost.DateClock, Boost.Chrono or std::chrono.
#if defined(NET_TS_HAS_BOOST_DATE_TIME)
  typedef WaitTraits traits_helper;
#else
  typedef detail::chrono_time_traits<Clock, WaitTraits> traits_helper;
#endif

public:
  /// The protocol type.
  typedef Protocol protocol_type;

  /// The endpoint type.
  typedef typename Protocol::endpoint endpoint_type;

  /// The clock type.
  typedef Clock clock_type;

#if defined(GENERATING_DOCUMENTATION)
  /// (Deprecated: Use time_point.) The time type.
  typedef typename WaitTraits::time_type time_type;

  /// The time type.
  typedef typename WaitTraits::time_point time_point;

  /// (Deprecated: Use duration.) The duration type.
  typedef typename WaitTraits::duration_type duration_type;

  /// The duration type.
  typedef typename WaitTraits::duration duration;
#else
  typedef typename traits_helper::time_type time_point;
  typedef typename traits_helper::duration_type duration;
#endif

  /// Construct a basic_socket_iostream without establishing a connection.
  basic_socket_iostream()
    : std::basic_iostream<char>(
        &this->detail::socket_iostream_base<
          Protocol NET_TS_SVC_TARG, Clock,
          WaitTraits NET_TS_SVC_TARG1>::streambuf_)
  {
    this->setf(std::ios_base::unitbuf);
  }

#if defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Construct a basic_socket_iostream from the supplied socket.
  explicit basic_socket_iostream(basic_stream_socket<protocol_type> s)
    : detail::socket_iostream_base<
        Protocol NET_TS_SVC_TARG, Clock,
        WaitTraits NET_TS_SVC_TARG1>(std::move(s)),
      std::basic_iostream<char>(
        &this->detail::socket_iostream_base<
          Protocol NET_TS_SVC_TARG, Clock,
          WaitTraits NET_TS_SVC_TARG1>::streambuf_)
  {
    this->setf(std::ios_base::unitbuf);
  }

#if defined(NET_TS_HAS_STD_IOSTREAM_MOVE) \
  || defined(GENERATING_DOCUMENTATION)
  /// Move-construct a basic_socket_iostream from another.
  basic_socket_iostream(basic_socket_iostream&& other)
    : detail::socket_iostream_base<
        Protocol NET_TS_SVC_TARG, Clock,
        WaitTraits NET_TS_SVC_TARG1>(std::move(other)),
      std::basic_iostream<char>(std::move(other))
  {
    this->set_rdbuf(&this->detail::socket_iostream_base<
          Protocol NET_TS_SVC_TARG, Clock,
          WaitTraits NET_TS_SVC_TARG1>::streambuf_);
  }

  /// Move-assign a basic_socket_iostream from another.
  basic_socket_iostream& operator=(basic_socket_iostream&& other)
  {
    std::basic_iostream<char>::operator=(std::move(other));
    detail::socket_iostream_base<
        Protocol NET_TS_SVC_TARG, Clock,
        WaitTraits NET_TS_SVC_TARG1>::operator=(std::move(other));
    return *this;
  }
#endif // defined(NET_TS_HAS_STD_IOSTREAM_MOVE)
       //   || defined(GENERATING_DOCUMENTATION)
#endif // defined(NET_TS_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

#if defined(GENERATING_DOCUMENTATION)
  /// Establish a connection to an endpoint corresponding to a resolver query.
  /**
   * This constructor automatically establishes a connection based on the
   * supplied resolver query parameters. The arguments are used to construct
   * a resolver query object.
   */
  template <typename T1, ..., typename TN>
  explicit basic_socket_iostream(T1 t1, ..., TN tn);
#elif defined(NET_TS_HAS_VARIADIC_TEMPLATES)
  template <typename... T>
  explicit basic_socket_iostream(T... x)
    : std::basic_iostream<char>(
        &this->detail::socket_iostream_base<
          Protocol NET_TS_SVC_TARG, Clock,
          WaitTraits NET_TS_SVC_TARG1>::streambuf_)
  {
    this->setf(std::ios_base::unitbuf);
    if (rdbuf()->connect(x...) == 0)
      this->setstate(std::ios_base::failbit);
  }
#else
  NET_TS_VARIADIC_GENERATE(NET_TS_PRIVATE_CTR_DEF)
#endif

#if defined(GENERATING_DOCUMENTATION)
  /// Establish a connection to an endpoint corresponding to a resolver query.
  /**
   * This function automatically establishes a connection based on the supplied
   * resolver query parameters. The arguments are used to construct a resolver
   * query object.
   */
  template <typename T1, ..., typename TN>
  void connect(T1 t1, ..., TN tn);
#elif defined(NET_TS_HAS_VARIADIC_TEMPLATES)
  template <typename... T>
  void connect(T... x)
  {
    if (rdbuf()->connect(x...) == 0)
      this->setstate(std::ios_base::failbit);
  }
#else
  NET_TS_VARIADIC_GENERATE(NET_TS_PRIVATE_CONNECT_DEF)
#endif

  /// Close the connection.
  void close()
  {
    if (rdbuf()->close() == 0)
      this->setstate(std::ios_base::failbit);
  }

  /// Return a pointer to the underlying streambuf.
  basic_socket_streambuf<Protocol NET_TS_SVC_TARG,
    Clock, WaitTraits NET_TS_SVC_TARG1>* rdbuf() const
  {
    return const_cast<basic_socket_streambuf<Protocol NET_TS_SVC_TARG,
      Clock, WaitTraits NET_TS_SVC_TARG1>*>(
        &this->detail::socket_iostream_base<
          Protocol NET_TS_SVC_TARG, Clock,
          WaitTraits NET_TS_SVC_TARG1>::streambuf_);
  }

  /// Get a reference to the underlying socket.
  basic_socket<Protocol NET_TS_SVC_TARG>& socket()
  {
    return rdbuf()->socket();
  }

  /// Get the last error associated with the stream.
  /**
   * @return An \c error_code corresponding to the last error from the stream.
   *
   * @par Example
   * To print the error associated with a failure to establish a connection:
   * @code tcp::iostream s("www.boost.org", "http");
   * if (!s)
   * {
   *   std::cout << "Error: " << s.error().message() << std::endl;
   * } @endcode
   */
  const std::error_code& error() const
  {
    return rdbuf()->error();
  }

  /// Get the stream's expiry time as an absolute time.
  /**
   * @return An absolute time value representing the stream's expiry time.
   */
  time_point expiry() const
  {
    return rdbuf()->expiry();
  }

  /// Set the stream's expiry time as an absolute time.
  /**
   * This function sets the expiry time associated with the stream. Stream
   * operations performed after this time (where the operations cannot be
   * completed using the internal buffers) will fail with the error
   * std::experimental::net::v1::error::operation_aborted.
   *
   * @param expiry_time The expiry time to be used for the stream.
   */
  void expires_at(const time_point& expiry_time)
  {
    rdbuf()->expires_at(expiry_time);
  }

  /// Set the stream's expiry time relative to now.
  /**
   * This function sets the expiry time associated with the stream. Stream
   * operations performed after this time (where the operations cannot be
   * completed using the internal buffers) will fail with the error
   * std::experimental::net::v1::error::operation_aborted.
   *
   * @param expiry_time The expiry time to be used for the timer.
   */
  void expires_after(const duration& expiry_time)
  {
    rdbuf()->expires_after(expiry_time);
  }

private:
  // Disallow copying and assignment.
  basic_socket_iostream(const basic_socket_iostream&) NET_TS_DELETED;
  basic_socket_iostream& operator=(
      const basic_socket_iostream&) NET_TS_DELETED;
};

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#if !defined(NET_TS_HAS_VARIADIC_TEMPLATES)
# undef NET_TS_PRIVATE_CTR_DEF
# undef NET_TS_PRIVATE_CONNECT_DEF
#endif // !defined(NET_TS_HAS_VARIADIC_TEMPLATES)

#endif // !defined(NET_TS_NO_IOSTREAM)

#endif // NET_TS_BASIC_SOCKET_IOSTREAM_HPP
