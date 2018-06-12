//
// read_until.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_READ_UNTIL_HPP
#define NET_TS_READ_UNTIL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <cstddef>
#include <string>
#include <experimental/__net_ts/async_result.hpp>
#include <experimental/__net_ts/detail/regex_fwd.hpp>
#include <experimental/__net_ts/detail/string_view.hpp>
#include <experimental/__net_ts/detail/type_traits.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

namespace detail
{
  char (&has_result_type_helper(...))[2];

  template <typename T>
  char has_result_type_helper(T*, typename T::result_type* = 0);

  template <typename T>
  struct has_result_type
  {
    enum { value = (sizeof((has_result_type_helper)((T*)(0))) == 1) };
  };
} // namespace detail

/// Type trait used to determine whether a type can be used as a match condition
/// function with read_until and async_read_until.
template <typename T>
struct is_match_condition
{
#if defined(GENERATING_DOCUMENTATION)
  /// The value member is true if the type may be used as a match condition.
  static const bool value;
#else
  enum
  {
    value = std::experimental::net::v1::is_function<
        typename std::experimental::net::v1::remove_pointer<T>::type>::value
      || detail::has_result_type<T>::value
  };
#endif
};

/**
 * @defgroup read_until std::experimental::net::v1::read_until
 *
 * @brief Read data into a dynamic buffer sequence, or into a streambuf, until
 * it contains a delimiter, matches a regular expression, or a function object
 * indicates a match.
 */
/*@{*/

/// Read data into a dynamic buffer sequence until it contains a specified
/// delimiter.
/**
 * This function is used to read data into the specified dynamic buffer
 * sequence until the dynamic buffer sequence's get area contains the specified
 * delimiter. The call will block until one of the following conditions is
 * true:
 *
 * @li The get area of the dynamic buffer sequence contains the specified
 * delimiter.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * read_some function. If the dynamic buffer sequence's get area already
 * contains the delimiter, the function returns immediately.
 *
 * @param s The stream from which the data is to be read. The type must support
 * the SyncReadStream concept.
 *
 * @param buffers The dynamic buffer sequence into which the data will be read.
 *
 * @param delim The delimiter character.
 *
 * @returns The number of bytes in the dynamic buffer sequence's get area up to
 * and including the delimiter.
 *
 * @throws std::system_error Thrown on failure.
 *
 * @note After a successful read_until operation, the dynamic buffer sequence
 * may contain additional data beyond the delimiter. An application will
 * typically leave that data in the dynamic buffer sequence for a subsequent
 * read_until operation to examine.
 *
 * @par Example
 * To read data into a @c std::string until a newline is encountered:
 * @code std::string data;
 * std::string n = std::experimental::net::read_until(s,
 *     std::experimental::net::dynamic_buffer(data), '\n');
 * std::string line = data.substr(0, n);
 * data.erase(0, n); @endcode
 * After the @c read_until operation completes successfully, the string @c data
 * contains the delimiter:
 * @code { 'a', 'b', ..., 'c', '\n', 'd', 'e', ... } @endcode
 * The call to @c substr then extracts the data up to and including the
 * delimiter, so that the string @c line contains:
 * @code { 'a', 'b', ..., 'c', '\n' } @endcode
 * After the call to @c erase, the remaining data is left in the buffer @c b as
 * follows:
 * @code { 'd', 'e', ... } @endcode
 * This data may be the start of a new line, to be extracted by a subsequent
 * @c read_until operation.
 */
template <typename SyncReadStream, typename DynamicBuffer>
std::size_t read_until(SyncReadStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers, char delim);

/// Read data into a dynamic buffer sequence until it contains a specified
/// delimiter.
/**
 * This function is used to read data into the specified dynamic buffer
 * sequence until the dynamic buffer sequence's get area contains the specified
 * delimiter. The call will block until one of the following conditions is
 * true:
 *
 * @li The get area of the dynamic buffer sequence contains the specified
 * delimiter.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * read_some function. If the dynamic buffer sequence's get area already
 * contains the delimiter, the function returns immediately.
 *
 * @param s The stream from which the data is to be read. The type must support
 * the SyncReadStream concept.
 *
 * @param buffers The dynamic buffer sequence into which the data will be read.
 *
 * @param delim The delimiter character.
 *
 * @param ec Set to indicate what error occurred, if any.
 *
 * @returns The number of bytes in the dynamic buffer sequence's get area up to
 * and including the delimiter. Returns 0 if an error occurred.
 *
 * @note After a successful read_until operation, the dynamic buffer sequence
 * may contain additional data beyond the delimiter. An application will
 * typically leave that data in the dynamic buffer sequence for a subsequent
 * read_until operation to examine.
 */
template <typename SyncReadStream, typename DynamicBuffer>
std::size_t read_until(SyncReadStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    char delim, std::error_code& ec);

/// Read data into a dynamic buffer sequence until it contains a specified
/// delimiter.
/**
 * This function is used to read data into the specified dynamic buffer
 * sequence until the dynamic buffer sequence's get area contains the specified
 * delimiter. The call will block until one of the following conditions is
 * true:
 *
 * @li The get area of the dynamic buffer sequence contains the specified
 * delimiter.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * read_some function. If the dynamic buffer sequence's get area already
 * contains the delimiter, the function returns immediately.
 *
 * @param s The stream from which the data is to be read. The type must support
 * the SyncReadStream concept.
 *
 * @param buffers The dynamic buffer sequence into which the data will be read.
 *
 * @param delim The delimiter string.
 *
 * @returns The number of bytes in the dynamic buffer sequence's get area up to
 * and including the delimiter.
 *
 * @note After a successful read_until operation, the dynamic buffer sequence
 * may contain additional data beyond the delimiter. An application will
 * typically leave that data in the dynamic buffer sequence for a subsequent
 * read_until operation to examine.
 *
 * @par Example
 * To read data into a @c std::string until a CR-LF sequence is encountered:
 * @code std::string data;
 * std::string n = std::experimental::net::read_until(s,
 *     std::experimental::net::dynamic_buffer(data), "\r\n");
 * std::string line = data.substr(0, n);
 * data.erase(0, n); @endcode
 * After the @c read_until operation completes successfully, the string @c data
 * contains the delimiter:
 * @code { 'a', 'b', ..., 'c', '\r', '\n', 'd', 'e', ... } @endcode
 * The call to @c substr then extracts the data up to and including the
 * delimiter, so that the string @c line contains:
 * @code { 'a', 'b', ..., 'c', '\r', '\n' } @endcode
 * After the call to @c erase, the remaining data is left in the buffer @c b as
 * follows:
 * @code { 'd', 'e', ... } @endcode
 * This data may be the start of a new line, to be extracted by a subsequent
 * @c read_until operation.
 */
template <typename SyncReadStream, typename DynamicBuffer>
std::size_t read_until(SyncReadStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    NET_TS_STRING_VIEW_PARAM delim);

/// Read data into a dynamic buffer sequence until it contains a specified
/// delimiter.
/**
 * This function is used to read data into the specified dynamic buffer
 * sequence until the dynamic buffer sequence's get area contains the specified
 * delimiter. The call will block until one of the following conditions is
 * true:
 *
 * @li The get area of the dynamic buffer sequence contains the specified
 * delimiter.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * read_some function. If the dynamic buffer sequence's get area already
 * contains the delimiter, the function returns immediately.
 *
 * @param s The stream from which the data is to be read. The type must support
 * the SyncReadStream concept.
 *
 * @param buffers The dynamic buffer sequence into which the data will be read.
 *
 * @param delim The delimiter string.
 *
 * @param ec Set to indicate what error occurred, if any.
 *
 * @returns The number of bytes in the dynamic buffer sequence's get area up to
 * and including the delimiter. Returns 0 if an error occurred.
 *
 * @note After a successful read_until operation, the dynamic buffer sequence
 * may contain additional data beyond the delimiter. An application will
 * typically leave that data in the dynamic buffer sequence for a subsequent
 * read_until operation to examine.
 */
template <typename SyncReadStream, typename DynamicBuffer>
std::size_t read_until(SyncReadStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    NET_TS_STRING_VIEW_PARAM delim,
    std::error_code& ec);

/*@}*/
/**
 * @defgroup async_read_until std::experimental::net::v1::async_read_until
 *
 * @brief Start an asynchronous operation to read data into a dynamic buffer
 * sequence, or into a streambuf, until it contains a delimiter, matches a
 * regular expression, or a function object indicates a match.
 */
/*@{*/

/// Start an asynchronous operation to read data into a dynamic buffer sequence
/// until it contains a specified delimiter.
/**
 * This function is used to asynchronously read data into the specified dynamic
 * buffer sequence until the dynamic buffer sequence's get area contains the
 * specified delimiter. The function call always returns immediately. The
 * asynchronous operation will continue until one of the following conditions
 * is true:
 *
 * @li The get area of the dynamic buffer sequence contains the specified
 * delimiter.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * async_read_some function, and is known as a <em>composed operation</em>. If
 * the dynamic buffer sequence's get area already contains the delimiter, this
 * asynchronous operation completes immediately. The program must ensure that
 * the stream performs no other read operations (such as async_read,
 * async_read_until, the stream's async_read_some function, or any other
 * composed operations that perform reads) until this operation completes.
 *
 * @param s The stream from which the data is to be read. The type must support
 * the AsyncReadStream concept.
 *
 * @param buffers The dynamic buffer sequence into which the data will be read.
 * Although the buffers object may be copied as necessary, ownership of the
 * underlying memory blocks is retained by the caller, which must guarantee
 * that they remain valid until the handler is called.
 *
 * @param delim The delimiter character.
 *
 * @param handler The handler to be called when the read operation completes.
 * Copies will be made of the handler as required. The function signature of the
 * handler must be:
 * @code void handler(
 *   // Result of operation.
 *   const std::error_code& error,
 *
 *   // The number of bytes in the dynamic buffer sequence's
 *   // get area up to and including the delimiter.
 *   // 0 if an error occurred.
 *   std::size_t bytes_transferred
 * ); @endcode
 * Regardless of whether the asynchronous operation completes immediately or
 * not, the handler will not be invoked from within this function. Invocation of
 * the handler will be performed in a manner equivalent to using
 * std::experimental::net::v1::io_context::post().
 *
 * @note After a successful async_read_until operation, the dynamic buffer
 * sequence may contain additional data beyond the delimiter. An application
 * will typically leave that data in the dynamic buffer sequence for a
 * subsequent async_read_until operation to examine.
 *
 * @par Example
 * To asynchronously read data into a @c std::string until a newline is
 * encountered:
 * @code std::string data;
 * ...
 * void handler(const std::error_code& e, std::size_t size)
 * {
 *   if (!e)
 *   {
 *     std::string line = data.substr(0, n);
 *     data.erase(0, n);
 *     ...
 *   }
 * }
 * ...
 * std::experimental::net::async_read_until(s, data, '\n', handler); @endcode
 * After the @c async_read_until operation completes successfully, the buffer
 * @c data contains the delimiter:
 * @code { 'a', 'b', ..., 'c', '\n', 'd', 'e', ... } @endcode
 * The call to @c substr then extracts the data up to and including the
 * delimiter, so that the string @c line contains:
 * @code { 'a', 'b', ..., 'c', '\n' } @endcode
 * After the call to @c erase, the remaining data is left in the buffer @c data
 * as follows:
 * @code { 'd', 'e', ... } @endcode
 * This data may be the start of a new line, to be extracted by a subsequent
 * @c async_read_until operation.
 */
template <typename AsyncReadStream,
    typename DynamicBuffer, typename ReadHandler>
NET_TS_INITFN_RESULT_TYPE(ReadHandler,
    void (std::error_code, std::size_t))
async_read_until(AsyncReadStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    char delim, NET_TS_MOVE_ARG(ReadHandler) handler);

/// Start an asynchronous operation to read data into a dynamic buffer sequence
/// until it contains a specified delimiter.
/**
 * This function is used to asynchronously read data into the specified dynamic
 * buffer sequence until the dynamic buffer sequence's get area contains the
 * specified delimiter. The function call always returns immediately. The
 * asynchronous operation will continue until one of the following conditions
 * is true:
 *
 * @li The get area of the dynamic buffer sequence contains the specified
 * delimiter.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * async_read_some function, and is known as a <em>composed operation</em>. If
 * the dynamic buffer sequence's get area already contains the delimiter, this
 * asynchronous operation completes immediately. The program must ensure that
 * the stream performs no other read operations (such as async_read,
 * async_read_until, the stream's async_read_some function, or any other
 * composed operations that perform reads) until this operation completes.
 *
 * @param s The stream from which the data is to be read. The type must support
 * the AsyncReadStream concept.
 *
 * @param buffers The dynamic buffer sequence into which the data will be read.
 * Although the buffers object may be copied as necessary, ownership of the
 * underlying memory blocks is retained by the caller, which must guarantee
 * that they remain valid until the handler is called.
 *
 * @param delim The delimiter string.
 *
 * @param handler The handler to be called when the read operation completes.
 * Copies will be made of the handler as required. The function signature of the
 * handler must be:
 * @code void handler(
 *   // Result of operation.
 *   const std::error_code& error,
 *
 *   // The number of bytes in the dynamic buffer sequence's
 *   // get area up to and including the delimiter.
 *   // 0 if an error occurred.
 *   std::size_t bytes_transferred
 * ); @endcode
 * Regardless of whether the asynchronous operation completes immediately or
 * not, the handler will not be invoked from within this function. Invocation of
 * the handler will be performed in a manner equivalent to using
 * std::experimental::net::v1::io_context::post().
 *
 * @note After a successful async_read_until operation, the dynamic buffer
 * sequence may contain additional data beyond the delimiter. An application
 * will typically leave that data in the dynamic buffer sequence for a
 * subsequent async_read_until operation to examine.
 *
 * @par Example
 * To asynchronously read data into a @c std::string until a CR-LF sequence is
 * encountered:
 * @code std::string data;
 * ...
 * void handler(const std::error_code& e, std::size_t size)
 * {
 *   if (!e)
 *   {
 *     std::string line = data.substr(0, n);
 *     data.erase(0, n);
 *     ...
 *   }
 * }
 * ...
 * std::experimental::net::async_read_until(s, data, "\r\n", handler); @endcode
 * After the @c async_read_until operation completes successfully, the string
 * @c data contains the delimiter:
 * @code { 'a', 'b', ..., 'c', '\r', '\n', 'd', 'e', ... } @endcode
 * The call to @c substr then extracts the data up to and including the
 * delimiter, so that the string @c line contains:
 * @code { 'a', 'b', ..., 'c', '\r', '\n' } @endcode
 * After the call to @c erase, the remaining data is left in the string @c data
 * as follows:
 * @code { 'd', 'e', ... } @endcode
 * This data may be the start of a new line, to be extracted by a subsequent
 * @c async_read_until operation.
 */
template <typename AsyncReadStream,
    typename DynamicBuffer, typename ReadHandler>
NET_TS_INITFN_RESULT_TYPE(ReadHandler,
    void (std::error_code, std::size_t))
async_read_until(AsyncReadStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    NET_TS_STRING_VIEW_PARAM delim,
    NET_TS_MOVE_ARG(ReadHandler) handler);

/*@}*/

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#include <experimental/__net_ts/impl/read_until.hpp>

#endif // NET_TS_READ_UNTIL_HPP
