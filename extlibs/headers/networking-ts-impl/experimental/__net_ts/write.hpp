//
// write.hpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_WRITE_HPP
#define NET_TS_WRITE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>
#include <cstddef>
#include <experimental/__net_ts/async_result.hpp>
#include <experimental/__net_ts/buffer.hpp>
#include <experimental/__net_ts/error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

/**
 * @defgroup write std::experimental::net::v1::write
 *
 * @brief Write a certain amount of data to a stream before returning.
 */
/*@{*/

/// Write all of the supplied data to a stream before returning.
/**
 * This function is used to write a certain number of bytes of data to a stream.
 * The call will block until one of the following conditions is true:
 *
 * @li All of the data in the supplied buffers has been written. That is, the
 * bytes transferred is equal to the sum of the buffer sizes.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * write_some function.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the SyncWriteStream concept.
 *
 * @param buffers One or more buffers containing the data to be written. The sum
 * of the buffer sizes indicates the maximum number of bytes to write to the
 * stream.
 *
 * @returns The number of bytes transferred.
 *
 * @throws std::system_error Thrown on failure.
 *
 * @par Example
 * To write a single data buffer use the @ref buffer function as follows:
 * @code std::experimental::net::write(s, std::experimental::net::buffer(data, size)); @endcode
 * See the @ref buffer documentation for information on writing multiple
 * buffers in one go, and how to use it with arrays, boost::array or
 * std::vector.
 *
 * @note This overload is equivalent to calling:
 * @code std::experimental::net::write(
 *     s, buffers,
 *     std::experimental::net::transfer_all()); @endcode
 */
template <typename SyncWriteStream, typename ConstBufferSequence>
std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type* = 0);

/// Write all of the supplied data to a stream before returning.
/**
 * This function is used to write a certain number of bytes of data to a stream.
 * The call will block until one of the following conditions is true:
 *
 * @li All of the data in the supplied buffers has been written. That is, the
 * bytes transferred is equal to the sum of the buffer sizes.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * write_some function.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the SyncWriteStream concept.
 *
 * @param buffers One or more buffers containing the data to be written. The sum
 * of the buffer sizes indicates the maximum number of bytes to write to the
 * stream.
 *
 * @param ec Set to indicate what error occurred, if any.
 *
 * @returns The number of bytes transferred.
 *
 * @par Example
 * To write a single data buffer use the @ref buffer function as follows:
 * @code std::experimental::net::write(s, std::experimental::net::buffer(data, size), ec); @endcode
 * See the @ref buffer documentation for information on writing multiple
 * buffers in one go, and how to use it with arrays, boost::array or
 * std::vector.
 *
 * @note This overload is equivalent to calling:
 * @code std::experimental::net::write(
 *     s, buffers,
 *     std::experimental::net::transfer_all(), ec); @endcode
 */
template <typename SyncWriteStream, typename ConstBufferSequence>
std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers,
    std::error_code& ec,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type* = 0);

/// Write a certain amount of data to a stream before returning.
/**
 * This function is used to write a certain number of bytes of data to a stream.
 * The call will block until one of the following conditions is true:
 *
 * @li All of the data in the supplied buffers has been written. That is, the
 * bytes transferred is equal to the sum of the buffer sizes.
 *
 * @li The completion_condition function object returns 0.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * write_some function.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the SyncWriteStream concept.
 *
 * @param buffers One or more buffers containing the data to be written. The sum
 * of the buffer sizes indicates the maximum number of bytes to write to the
 * stream.
 *
 * @param completion_condition The function object to be called to determine
 * whether the write operation is complete. The signature of the function object
 * must be:
 * @code std::size_t completion_condition(
 *   // Result of latest write_some operation.
 *   const std::error_code& error,
 *
 *   // Number of bytes transferred so far.
 *   std::size_t bytes_transferred
 * ); @endcode
 * A return value of 0 indicates that the write operation is complete. A
 * non-zero return value indicates the maximum number of bytes to be written on
 * the next call to the stream's write_some function.
 *
 * @returns The number of bytes transferred.
 *
 * @throws std::system_error Thrown on failure.
 *
 * @par Example
 * To write a single data buffer use the @ref buffer function as follows:
 * @code std::experimental::net::write(s, std::experimental::net::buffer(data, size),
 *     std::experimental::net::transfer_at_least(32)); @endcode
 * See the @ref buffer documentation for information on writing multiple
 * buffers in one go, and how to use it with arrays, boost::array or
 * std::vector.
 */
template <typename SyncWriteStream, typename ConstBufferSequence,
    typename CompletionCondition>
std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers,
    CompletionCondition completion_condition,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type* = 0);

/// Write a certain amount of data to a stream before returning.
/**
 * This function is used to write a certain number of bytes of data to a stream.
 * The call will block until one of the following conditions is true:
 *
 * @li All of the data in the supplied buffers has been written. That is, the
 * bytes transferred is equal to the sum of the buffer sizes.
 *
 * @li The completion_condition function object returns 0.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * write_some function.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the SyncWriteStream concept.
 *
 * @param buffers One or more buffers containing the data to be written. The sum
 * of the buffer sizes indicates the maximum number of bytes to write to the
 * stream.
 *
 * @param completion_condition The function object to be called to determine
 * whether the write operation is complete. The signature of the function object
 * must be:
 * @code std::size_t completion_condition(
 *   // Result of latest write_some operation.
 *   const std::error_code& error,
 *
 *   // Number of bytes transferred so far.
 *   std::size_t bytes_transferred
 * ); @endcode
 * A return value of 0 indicates that the write operation is complete. A
 * non-zero return value indicates the maximum number of bytes to be written on
 * the next call to the stream's write_some function.
 *
 * @param ec Set to indicate what error occurred, if any.
 *
 * @returns The number of bytes written. If an error occurs, returns the total
 * number of bytes successfully transferred prior to the error.
 */
template <typename SyncWriteStream, typename ConstBufferSequence,
    typename CompletionCondition>
std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers,
    CompletionCondition completion_condition, std::error_code& ec,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type* = 0);

/// Write all of the supplied data to a stream before returning.
/**
 * This function is used to write a certain number of bytes of data to a stream.
 * The call will block until one of the following conditions is true:
 *
 * @li All of the data in the supplied dynamic buffer sequence has been written.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * write_some function.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the SyncWriteStream concept.
 *
 * @param buffers The dynamic buffer sequence from which data will be written.
 * Successfully written data is automatically consumed from the buffers.
 *
 * @returns The number of bytes transferred.
 *
 * @throws std::system_error Thrown on failure.
 *
 * @note This overload is equivalent to calling:
 * @code std::experimental::net::write(
 *     s, buffers,
 *     std::experimental::net::transfer_all()); @endcode
 */
template <typename SyncWriteStream, typename DynamicBuffer>
std::size_t write(SyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type* = 0);

/// Write all of the supplied data to a stream before returning.
/**
 * This function is used to write a certain number of bytes of data to a stream.
 * The call will block until one of the following conditions is true:
 *
 * @li All of the data in the supplied dynamic buffer sequence has been written.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * write_some function.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the SyncWriteStream concept.
 *
 * @param buffers The dynamic buffer sequence from which data will be written.
 * Successfully written data is automatically consumed from the buffers.
 *
 * @param ec Set to indicate what error occurred, if any.
 *
 * @returns The number of bytes transferred.
 *
 * @note This overload is equivalent to calling:
 * @code std::experimental::net::write(
 *     s, buffers,
 *     std::experimental::net::transfer_all(), ec); @endcode
 */
template <typename SyncWriteStream, typename DynamicBuffer>
std::size_t write(SyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    std::error_code& ec,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type* = 0);

/// Write a certain amount of data to a stream before returning.
/**
 * This function is used to write a certain number of bytes of data to a stream.
 * The call will block until one of the following conditions is true:
 *
 * @li All of the data in the supplied dynamic buffer sequence has been written.
 *
 * @li The completion_condition function object returns 0.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * write_some function.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the SyncWriteStream concept.
 *
 * @param buffers The dynamic buffer sequence from which data will be written.
 * Successfully written data is automatically consumed from the buffers.
 *
 * @param completion_condition The function object to be called to determine
 * whether the write operation is complete. The signature of the function object
 * must be:
 * @code std::size_t completion_condition(
 *   // Result of latest write_some operation.
 *   const std::error_code& error,
 *
 *   // Number of bytes transferred so far.
 *   std::size_t bytes_transferred
 * ); @endcode
 * A return value of 0 indicates that the write operation is complete. A
 * non-zero return value indicates the maximum number of bytes to be written on
 * the next call to the stream's write_some function.
 *
 * @returns The number of bytes transferred.
 *
 * @throws std::system_error Thrown on failure.
 */
template <typename SyncWriteStream, typename DynamicBuffer,
    typename CompletionCondition>
std::size_t write(SyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    CompletionCondition completion_condition,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type* = 0);

/// Write a certain amount of data to a stream before returning.
/**
 * This function is used to write a certain number of bytes of data to a stream.
 * The call will block until one of the following conditions is true:
 *
 * @li All of the data in the supplied dynamic buffer sequence has been written.
 *
 * @li The completion_condition function object returns 0.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * write_some function.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the SyncWriteStream concept.
 *
 * @param buffers The dynamic buffer sequence from which data will be written.
 * Successfully written data is automatically consumed from the buffers.
 *
 * @param completion_condition The function object to be called to determine
 * whether the write operation is complete. The signature of the function object
 * must be:
 * @code std::size_t completion_condition(
 *   // Result of latest write_some operation.
 *   const std::error_code& error,
 *
 *   // Number of bytes transferred so far.
 *   std::size_t bytes_transferred
 * ); @endcode
 * A return value of 0 indicates that the write operation is complete. A
 * non-zero return value indicates the maximum number of bytes to be written on
 * the next call to the stream's write_some function.
 *
 * @param ec Set to indicate what error occurred, if any.
 *
 * @returns The number of bytes written. If an error occurs, returns the total
 * number of bytes successfully transferred prior to the error.
 */
template <typename SyncWriteStream, typename DynamicBuffer,
    typename CompletionCondition>
std::size_t write(SyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    CompletionCondition completion_condition, std::error_code& ec,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type* = 0);

/*@}*/
/**
 * @defgroup async_write std::experimental::net::v1::async_write
 *
 * @brief Start an asynchronous operation to write a certain amount of data to a
 * stream.
 */
/*@{*/

/// Start an asynchronous operation to write all of the supplied data to a
/// stream.
/**
 * This function is used to asynchronously write a certain number of bytes of
 * data to a stream. The function call always returns immediately. The
 * asynchronous operation will continue until one of the following conditions
 * is true:
 *
 * @li All of the data in the supplied buffers has been written. That is, the
 * bytes transferred is equal to the sum of the buffer sizes.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * async_write_some function, and is known as a <em>composed operation</em>. The
 * program must ensure that the stream performs no other write operations (such
 * as async_write, the stream's async_write_some function, or any other composed
 * operations that perform writes) until this operation completes.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the AsyncWriteStream concept.
 *
 * @param buffers One or more buffers containing the data to be written.
 * Although the buffers object may be copied as necessary, ownership of the
 * underlying memory blocks is retained by the caller, which must guarantee
 * that they remain valid until the handler is called.
 *
 * @param handler The handler to be called when the write operation completes.
 * Copies will be made of the handler as required. The function signature of
 * the handler must be:
 * @code void handler(
 *   const std::error_code& error, // Result of operation.
 *
 *   std::size_t bytes_transferred           // Number of bytes written from the
 *                                           // buffers. If an error occurred,
 *                                           // this will be less than the sum
 *                                           // of the buffer sizes.
 * ); @endcode
 * Regardless of whether the asynchronous operation completes immediately or
 * not, the handler will not be invoked from within this function. Invocation of
 * the handler will be performed in a manner equivalent to using
 * std::experimental::net::v1::io_context::post().
 *
 * @par Example
 * To write a single data buffer use the @ref buffer function as follows:
 * @code
 * std::experimental::net::async_write(s, std::experimental::net::buffer(data, size), handler);
 * @endcode
 * See the @ref buffer documentation for information on writing multiple
 * buffers in one go, and how to use it with arrays, boost::array or
 * std::vector.
 */
template <typename AsyncWriteStream, typename ConstBufferSequence,
    typename WriteHandler>
NET_TS_INITFN_RESULT_TYPE(WriteHandler,
    void (std::error_code, std::size_t))
async_write(AsyncWriteStream& s, const ConstBufferSequence& buffers,
    NET_TS_MOVE_ARG(WriteHandler) handler,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type* = 0);

/// Start an asynchronous operation to write a certain amount of data to a
/// stream.
/**
 * This function is used to asynchronously write a certain number of bytes of
 * data to a stream. The function call always returns immediately. The
 * asynchronous operation will continue until one of the following conditions
 * is true:
 *
 * @li All of the data in the supplied buffers has been written. That is, the
 * bytes transferred is equal to the sum of the buffer sizes.
 *
 * @li The completion_condition function object returns 0.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * async_write_some function, and is known as a <em>composed operation</em>. The
 * program must ensure that the stream performs no other write operations (such
 * as async_write, the stream's async_write_some function, or any other composed
 * operations that perform writes) until this operation completes.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the AsyncWriteStream concept.
 *
 * @param buffers One or more buffers containing the data to be written.
 * Although the buffers object may be copied as necessary, ownership of the
 * underlying memory blocks is retained by the caller, which must guarantee
 * that they remain valid until the handler is called.
 *
 * @param completion_condition The function object to be called to determine
 * whether the write operation is complete. The signature of the function object
 * must be:
 * @code std::size_t completion_condition(
 *   // Result of latest async_write_some operation.
 *   const std::error_code& error,
 *
 *   // Number of bytes transferred so far.
 *   std::size_t bytes_transferred
 * ); @endcode
 * A return value of 0 indicates that the write operation is complete. A
 * non-zero return value indicates the maximum number of bytes to be written on
 * the next call to the stream's async_write_some function.
 *
 * @param handler The handler to be called when the write operation completes.
 * Copies will be made of the handler as required. The function signature of the
 * handler must be:
 * @code void handler(
 *   const std::error_code& error, // Result of operation.
 *
 *   std::size_t bytes_transferred           // Number of bytes written from the
 *                                           // buffers. If an error occurred,
 *                                           // this will be less than the sum
 *                                           // of the buffer sizes.
 * ); @endcode
 * Regardless of whether the asynchronous operation completes immediately or
 * not, the handler will not be invoked from within this function. Invocation of
 * the handler will be performed in a manner equivalent to using
 * std::experimental::net::v1::io_context::post().
 *
 * @par Example
 * To write a single data buffer use the @ref buffer function as follows:
 * @code std::experimental::net::async_write(s,
 *     std::experimental::net::buffer(data, size),
 *     std::experimental::net::transfer_at_least(32),
 *     handler); @endcode
 * See the @ref buffer documentation for information on writing multiple
 * buffers in one go, and how to use it with arrays, boost::array or
 * std::vector.
 */
template <typename AsyncWriteStream, typename ConstBufferSequence,
    typename CompletionCondition, typename WriteHandler>
NET_TS_INITFN_RESULT_TYPE(WriteHandler,
    void (std::error_code, std::size_t))
async_write(AsyncWriteStream& s, const ConstBufferSequence& buffers,
    CompletionCondition completion_condition,
    NET_TS_MOVE_ARG(WriteHandler) handler,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type* = 0);

/// Start an asynchronous operation to write all of the supplied data to a
/// stream.
/**
 * This function is used to asynchronously write a certain number of bytes of
 * data to a stream. The function call always returns immediately. The
 * asynchronous operation will continue until one of the following conditions
 * is true:
 *
 * @li All of the data in the supplied dynamic buffer sequence has been written.
 *
 * @li An error occurred.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * async_write_some function, and is known as a <em>composed operation</em>. The
 * program must ensure that the stream performs no other write operations (such
 * as async_write, the stream's async_write_some function, or any other composed
 * operations that perform writes) until this operation completes.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the AsyncWriteStream concept.
 *
 * @param buffers The dynamic buffer sequence from which data will be written.
 * Although the buffers object may be copied as necessary, ownership of the
 * underlying memory blocks is retained by the caller, which must guarantee
 * that they remain valid until the handler is called. Successfully written
 * data is automatically consumed from the buffers.
 *
 * @param handler The handler to be called when the write operation completes.
 * Copies will be made of the handler as required. The function signature of the
 * handler must be:
 * @code void handler(
 *   const std::error_code& error, // Result of operation.
 *
 *   std::size_t bytes_transferred           // Number of bytes written from the
 *                                           // buffers. If an error occurred,
 *                                           // this will be less than the sum
 *                                           // of the buffer sizes.
 * ); @endcode
 * Regardless of whether the asynchronous operation completes immediately or
 * not, the handler will not be invoked from within this function. Invocation of
 * the handler will be performed in a manner equivalent to using
 * std::experimental::net::v1::io_context::post().
 */
template <typename AsyncWriteStream,
    typename DynamicBuffer, typename WriteHandler>
NET_TS_INITFN_RESULT_TYPE(WriteHandler,
    void (std::error_code, std::size_t))
async_write(AsyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    NET_TS_MOVE_ARG(WriteHandler) handler,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type* = 0);

/// Start an asynchronous operation to write a certain amount of data to a
/// stream.
/**
 * This function is used to asynchronously write a certain number of bytes of
 * data to a stream. The function call always returns immediately. The
 * asynchronous operation will continue until one of the following conditions
 * is true:
 *
 * @li All of the data in the supplied dynamic buffer sequence has been written.
 *
 * @li The completion_condition function object returns 0.
 *
 * This operation is implemented in terms of zero or more calls to the stream's
 * async_write_some function, and is known as a <em>composed operation</em>. The
 * program must ensure that the stream performs no other write operations (such
 * as async_write, the stream's async_write_some function, or any other composed
 * operations that perform writes) until this operation completes.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the AsyncWriteStream concept.
 *
 * @param buffers The dynamic buffer sequence from which data will be written.
 * Although the buffers object may be copied as necessary, ownership of the
 * underlying memory blocks is retained by the caller, which must guarantee
 * that they remain valid until the handler is called. Successfully written
 * data is automatically consumed from the buffers.
 *
 * @param completion_condition The function object to be called to determine
 * whether the write operation is complete. The signature of the function object
 * must be:
 * @code std::size_t completion_condition(
 *   // Result of latest async_write_some operation.
 *   const std::error_code& error,
 *
 *   // Number of bytes transferred so far.
 *   std::size_t bytes_transferred
 * ); @endcode
 * A return value of 0 indicates that the write operation is complete. A
 * non-zero return value indicates the maximum number of bytes to be written on
 * the next call to the stream's async_write_some function.
 *
 * @param handler The handler to be called when the write operation completes.
 * Copies will be made of the handler as required. The function signature of the
 * handler must be:
 * @code void handler(
 *   const std::error_code& error, // Result of operation.
 *
 *   std::size_t bytes_transferred           // Number of bytes written from the
 *                                           // buffers. If an error occurred,
 *                                           // this will be less than the sum
 *                                           // of the buffer sizes.
 * ); @endcode
 * Regardless of whether the asynchronous operation completes immediately or
 * not, the handler will not be invoked from within this function. Invocation of
 * the handler will be performed in a manner equivalent to using
 * std::experimental::net::v1::io_context::post().
 */
template <typename AsyncWriteStream, typename DynamicBuffer,
    typename CompletionCondition, typename WriteHandler>
NET_TS_INITFN_RESULT_TYPE(WriteHandler,
    void (std::error_code, std::size_t))
async_write(AsyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    CompletionCondition completion_condition,
    NET_TS_MOVE_ARG(WriteHandler) handler,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type* = 0);

/*@}*/

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#include <experimental/__net_ts/impl/write.hpp>

#endif // NET_TS_WRITE_HPP
