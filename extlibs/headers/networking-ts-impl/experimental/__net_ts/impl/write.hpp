//
// impl/write.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_IMPL_WRITE_HPP
#define NET_TS_IMPL_WRITE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/associated_allocator.hpp>
#include <experimental/__net_ts/associated_executor.hpp>
#include <experimental/__net_ts/buffer.hpp>
#include <experimental/__net_ts/completion_condition.hpp>
#include <experimental/__net_ts/detail/array_fwd.hpp>
#include <experimental/__net_ts/detail/base_from_completion_cond.hpp>
#include <experimental/__net_ts/detail/bind_handler.hpp>
#include <experimental/__net_ts/detail/consuming_buffers.hpp>
#include <experimental/__net_ts/detail/dependent_type.hpp>
#include <experimental/__net_ts/detail/handler_alloc_helpers.hpp>
#include <experimental/__net_ts/detail/handler_cont_helpers.hpp>
#include <experimental/__net_ts/detail/handler_invoke_helpers.hpp>
#include <experimental/__net_ts/detail/handler_type_requirements.hpp>
#include <experimental/__net_ts/detail/throw_error.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {

namespace detail
{
  template <typename SyncWriteStream, typename ConstBufferSequence,
      typename ConstBufferIterator, typename CompletionCondition>
  std::size_t write_buffer_sequence(SyncWriteStream& s,
      const ConstBufferSequence& buffers, const ConstBufferIterator&,
      CompletionCondition completion_condition, std::error_code& ec)
  {
    ec = std::error_code();
    std::experimental::net::v1::detail::consuming_buffers<const_buffer,
        ConstBufferSequence, ConstBufferIterator> tmp(buffers);
    while (!tmp.empty())
    {
      if (std::size_t max_size = detail::adapt_completion_condition_result(
            completion_condition(ec, tmp.total_consumed())))
        tmp.consume(s.write_some(tmp.prepare(max_size), ec));
      else
        break;
    }
    return tmp.total_consumed();;
  }
} // namespace detail

template <typename SyncWriteStream, typename ConstBufferSequence,
    typename CompletionCondition>
inline std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers,
    CompletionCondition completion_condition, std::error_code& ec,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type*)
{
  return detail::write_buffer_sequence(s, buffers,
      std::experimental::net::v1::buffer_sequence_begin(buffers), completion_condition, ec);
}

template <typename SyncWriteStream, typename ConstBufferSequence>
inline std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type*)
{
  std::error_code ec;
  std::size_t bytes_transferred = write(s, buffers, transfer_all(), ec);
  std::experimental::net::v1::detail::throw_error(ec, "write");
  return bytes_transferred;
}

template <typename SyncWriteStream, typename ConstBufferSequence>
inline std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers,
    std::error_code& ec,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type*)
{
  return write(s, buffers, transfer_all(), ec);
}

template <typename SyncWriteStream, typename ConstBufferSequence,
    typename CompletionCondition>
inline std::size_t write(SyncWriteStream& s, const ConstBufferSequence& buffers,
    CompletionCondition completion_condition,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type*)
{
  std::error_code ec;
  std::size_t bytes_transferred = write(s, buffers, completion_condition, ec);
  std::experimental::net::v1::detail::throw_error(ec, "write");
  return bytes_transferred;
}

template <typename SyncWriteStream, typename DynamicBuffer,
    typename CompletionCondition>
std::size_t write(SyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    CompletionCondition completion_condition, std::error_code& ec,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type*)
{
  typename decay<DynamicBuffer>::type b(
      NET_TS_MOVE_CAST(DynamicBuffer)(buffers));

  std::size_t bytes_transferred = write(s, b.data(), completion_condition, ec);
  b.consume(bytes_transferred);
  return bytes_transferred;
}

template <typename SyncWriteStream, typename DynamicBuffer>
inline std::size_t write(SyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type*)
{
  std::error_code ec;
  std::size_t bytes_transferred = write(s,
      NET_TS_MOVE_CAST(DynamicBuffer)(buffers),
      transfer_all(), ec);
  std::experimental::net::v1::detail::throw_error(ec, "write");
  return bytes_transferred;
}

template <typename SyncWriteStream, typename DynamicBuffer>
inline std::size_t write(SyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    std::error_code& ec,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type*)
{
  return write(s, NET_TS_MOVE_CAST(DynamicBuffer)(buffers),
      transfer_all(), ec);
}

template <typename SyncWriteStream, typename DynamicBuffer,
    typename CompletionCondition>
inline std::size_t write(SyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    CompletionCondition completion_condition,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type*)
{
  std::error_code ec;
  std::size_t bytes_transferred = write(s,
      NET_TS_MOVE_CAST(DynamicBuffer)(buffers),
      completion_condition, ec);
  std::experimental::net::v1::detail::throw_error(ec, "write");
  return bytes_transferred;
}

namespace detail
{
  template <typename AsyncWriteStream, typename ConstBufferSequence,
      typename ConstBufferIterator, typename CompletionCondition,
      typename WriteHandler>
  class write_op
    : detail::base_from_completion_cond<CompletionCondition>
  {
  public:
    write_op(AsyncWriteStream& stream, const ConstBufferSequence& buffers,
        CompletionCondition completion_condition, WriteHandler& handler)
      : detail::base_from_completion_cond<
          CompletionCondition>(completion_condition),
        stream_(stream),
        buffers_(buffers),
        start_(0),
        handler_(NET_TS_MOVE_CAST(WriteHandler)(handler))
    {
    }

#if defined(NET_TS_HAS_MOVE)
    write_op(const write_op& other)
      : detail::base_from_completion_cond<CompletionCondition>(other),
        stream_(other.stream_),
        buffers_(other.buffers_),
        start_(other.start_),
        handler_(other.handler_)
    {
    }

    write_op(write_op&& other)
      : detail::base_from_completion_cond<CompletionCondition>(other),
        stream_(other.stream_),
        buffers_(other.buffers_),
        start_(other.start_),
        handler_(NET_TS_MOVE_CAST(WriteHandler)(other.handler_))
    {
    }
#endif // defined(NET_TS_HAS_MOVE)

    void operator()(const std::error_code& ec,
        std::size_t bytes_transferred, int start = 0)
    {
      std::size_t max_size;
      switch (start_ = start)
      {
        case 1:
        max_size = this->check_for_completion(ec, buffers_.total_consumed());
        do
        {
          stream_.async_write_some(buffers_.prepare(max_size),
              NET_TS_MOVE_CAST(write_op)(*this));
          return; default:
          buffers_.consume(bytes_transferred);
          if ((!ec && bytes_transferred == 0) || buffers_.empty())
            break;
          max_size = this->check_for_completion(ec, buffers_.total_consumed());
        } while (max_size > 0);

        handler_(ec, buffers_.total_consumed());
      }
    }

  //private:
    AsyncWriteStream& stream_;
    std::experimental::net::v1::detail::consuming_buffers<const_buffer,
        ConstBufferSequence, ConstBufferIterator> buffers_;
    int start_;
    WriteHandler handler_;
  };

  template <typename AsyncWriteStream, typename ConstBufferSequence,
      typename ConstBufferIterator, typename CompletionCondition,
      typename WriteHandler>
  inline void* networking_ts_handler_allocate(std::size_t size,
      write_op<AsyncWriteStream, ConstBufferSequence, ConstBufferIterator,
        CompletionCondition, WriteHandler>* this_handler)
  {
    return networking_ts_handler_alloc_helpers::allocate(
        size, this_handler->handler_);
  }

  template <typename AsyncWriteStream, typename ConstBufferSequence,
      typename ConstBufferIterator, typename CompletionCondition,
      typename WriteHandler>
  inline void networking_ts_handler_deallocate(void* pointer, std::size_t size,
      write_op<AsyncWriteStream, ConstBufferSequence, ConstBufferIterator,
        CompletionCondition, WriteHandler>* this_handler)
  {
    networking_ts_handler_alloc_helpers::deallocate(
        pointer, size, this_handler->handler_);
  }

  template <typename AsyncWriteStream, typename ConstBufferSequence,
      typename ConstBufferIterator, typename CompletionCondition,
      typename WriteHandler>
  inline bool networking_ts_handler_is_continuation(
      write_op<AsyncWriteStream, ConstBufferSequence, ConstBufferIterator,
        CompletionCondition, WriteHandler>* this_handler)
  {
    return this_handler->start_ == 0 ? true
      : networking_ts_handler_cont_helpers::is_continuation(
          this_handler->handler_);
  }

  template <typename Function, typename AsyncWriteStream,
      typename ConstBufferSequence, typename ConstBufferIterator,
      typename CompletionCondition, typename WriteHandler>
  inline void networking_ts_handler_invoke(Function& function,
      write_op<AsyncWriteStream, ConstBufferSequence, ConstBufferIterator,
        CompletionCondition, WriteHandler>* this_handler)
  {
    networking_ts_handler_invoke_helpers::invoke(
        function, this_handler->handler_);
  }

  template <typename Function, typename AsyncWriteStream,
      typename ConstBufferSequence, typename ConstBufferIterator,
      typename CompletionCondition, typename WriteHandler>
  inline void networking_ts_handler_invoke(const Function& function,
      write_op<AsyncWriteStream, ConstBufferSequence, ConstBufferIterator,
        CompletionCondition, WriteHandler>* this_handler)
  {
    networking_ts_handler_invoke_helpers::invoke(
        function, this_handler->handler_);
  }

  template <typename AsyncWriteStream, typename ConstBufferSequence,
      typename ConstBufferIterator, typename CompletionCondition,
      typename WriteHandler>
  inline void start_write_buffer_sequence_op(AsyncWriteStream& stream,
      const ConstBufferSequence& buffers, const ConstBufferIterator&,
      CompletionCondition completion_condition, WriteHandler& handler)
  {
    detail::write_op<AsyncWriteStream, ConstBufferSequence,
      ConstBufferIterator, CompletionCondition, WriteHandler>(
        stream, buffers, completion_condition, handler)(
          std::error_code(), 0, 1);
  }

} // namespace detail

#if !defined(GENERATING_DOCUMENTATION)

template <typename AsyncWriteStream, typename ConstBufferSequence,
    typename ConstBufferIterator, typename CompletionCondition,
    typename WriteHandler, typename Allocator>
struct associated_allocator<
    detail::write_op<AsyncWriteStream, ConstBufferSequence,
      ConstBufferIterator, CompletionCondition, WriteHandler>,
    Allocator>
{
  typedef typename associated_allocator<WriteHandler, Allocator>::type type;

  static type get(
      const detail::write_op<AsyncWriteStream, ConstBufferSequence,
        ConstBufferIterator, CompletionCondition, WriteHandler>& h,
      const Allocator& a = Allocator()) NET_TS_NOEXCEPT
  {
    return associated_allocator<WriteHandler, Allocator>::get(h.handler_, a);
  }
};

template <typename AsyncWriteStream, typename ConstBufferSequence,
    typename ConstBufferIterator, typename CompletionCondition,
    typename WriteHandler, typename Executor>
struct associated_executor<
    detail::write_op<AsyncWriteStream, ConstBufferSequence,
      ConstBufferIterator, CompletionCondition, WriteHandler>,
    Executor>
{
  typedef typename associated_executor<WriteHandler, Executor>::type type;

  static type get(
      const detail::write_op<AsyncWriteStream, ConstBufferSequence,
        ConstBufferIterator, CompletionCondition, WriteHandler>& h,
      const Executor& ex = Executor()) NET_TS_NOEXCEPT
  {
    return associated_executor<WriteHandler, Executor>::get(h.handler_, ex);
  }
};

#endif // !defined(GENERATING_DOCUMENTATION)

template <typename AsyncWriteStream, typename ConstBufferSequence,
  typename CompletionCondition, typename WriteHandler>
inline NET_TS_INITFN_RESULT_TYPE(WriteHandler,
    void (std::error_code, std::size_t))
async_write(AsyncWriteStream& s, const ConstBufferSequence& buffers,
    CompletionCondition completion_condition,
    NET_TS_MOVE_ARG(WriteHandler) handler,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type*)
{
  // If you get an error on the following line it means that your handler does
  // not meet the documented type requirements for a WriteHandler.
  NET_TS_WRITE_HANDLER_CHECK(WriteHandler, handler) type_check;

  async_completion<WriteHandler,
    void (std::error_code, std::size_t)> init(handler);

  detail::start_write_buffer_sequence_op(s, buffers,
      std::experimental::net::v1::buffer_sequence_begin(buffers), completion_condition,
      init.completion_handler);

  return init.result.get();
}

template <typename AsyncWriteStream, typename ConstBufferSequence,
    typename WriteHandler>
inline NET_TS_INITFN_RESULT_TYPE(WriteHandler,
    void (std::error_code, std::size_t))
async_write(AsyncWriteStream& s, const ConstBufferSequence& buffers,
    NET_TS_MOVE_ARG(WriteHandler) handler,
    typename enable_if<
      is_const_buffer_sequence<ConstBufferSequence>::value
    >::type*)
{
  // If you get an error on the following line it means that your handler does
  // not meet the documented type requirements for a WriteHandler.
  NET_TS_WRITE_HANDLER_CHECK(WriteHandler, handler) type_check;

  async_completion<WriteHandler,
    void (std::error_code, std::size_t)> init(handler);

  detail::start_write_buffer_sequence_op(s, buffers,
      std::experimental::net::v1::buffer_sequence_begin(buffers), transfer_all(),
      init.completion_handler);

  return init.result.get();
}

namespace detail
{
  template <typename AsyncWriteStream, typename DynamicBuffer,
      typename CompletionCondition, typename WriteHandler>
  class write_dynbuf_op
  {
  public:
    template <typename BufferSequence>
    write_dynbuf_op(AsyncWriteStream& stream,
        NET_TS_MOVE_ARG(BufferSequence) buffers,
        CompletionCondition completion_condition, WriteHandler& handler)
      : stream_(stream),
        buffers_(NET_TS_MOVE_CAST(BufferSequence)(buffers)),
        completion_condition_(
          NET_TS_MOVE_CAST(CompletionCondition)(completion_condition)),
        handler_(NET_TS_MOVE_CAST(WriteHandler)(handler))
    {
    }

#if defined(NET_TS_HAS_MOVE)
    write_dynbuf_op(const write_dynbuf_op& other)
      : stream_(other.stream_),
        buffers_(other.buffers_),
        completion_condition_(other.completion_condition_),
        handler_(other.handler_)
    {
    }

    write_dynbuf_op(write_dynbuf_op&& other)
      : stream_(other.stream_),
        buffers_(NET_TS_MOVE_CAST(DynamicBuffer)(other.buffers_)),
        completion_condition_(
          NET_TS_MOVE_CAST(CompletionCondition)(
            other.completion_condition_)),
        handler_(NET_TS_MOVE_CAST(WriteHandler)(other.handler_))
    {
    }
#endif // defined(NET_TS_HAS_MOVE)

    void operator()(const std::error_code& ec,
        std::size_t bytes_transferred, int start = 0)
    {
      switch (start)
      {
        case 1:
        async_write(stream_, buffers_.data(), completion_condition_,
            NET_TS_MOVE_CAST(write_dynbuf_op)(*this));
        return; default:
        buffers_.consume(bytes_transferred);
        handler_(ec, static_cast<const std::size_t&>(bytes_transferred));
      }
    }

  //private:
    AsyncWriteStream& stream_;
    DynamicBuffer buffers_;
    CompletionCondition completion_condition_;
    WriteHandler handler_;
  };

  template <typename AsyncWriteStream, typename DynamicBuffer,
      typename CompletionCondition, typename WriteHandler>
  inline void* networking_ts_handler_allocate(std::size_t size,
      write_dynbuf_op<AsyncWriteStream, DynamicBuffer,
        CompletionCondition, WriteHandler>* this_handler)
  {
    return networking_ts_handler_alloc_helpers::allocate(
        size, this_handler->handler_);
  }

  template <typename AsyncWriteStream, typename DynamicBuffer,
      typename CompletionCondition, typename WriteHandler>
  inline void networking_ts_handler_deallocate(void* pointer, std::size_t size,
      write_dynbuf_op<AsyncWriteStream, DynamicBuffer,
        CompletionCondition, WriteHandler>* this_handler)
  {
    networking_ts_handler_alloc_helpers::deallocate(
        pointer, size, this_handler->handler_);
  }

  template <typename AsyncWriteStream, typename DynamicBuffer,
      typename CompletionCondition, typename WriteHandler>
  inline bool networking_ts_handler_is_continuation(
      write_dynbuf_op<AsyncWriteStream, DynamicBuffer,
        CompletionCondition, WriteHandler>* this_handler)
  {
    return networking_ts_handler_cont_helpers::is_continuation(
        this_handler->handler_);
  }

  template <typename Function, typename AsyncWriteStream,
      typename DynamicBuffer, typename CompletionCondition,
      typename WriteHandler>
  inline void networking_ts_handler_invoke(Function& function,
      write_dynbuf_op<AsyncWriteStream, DynamicBuffer,
        CompletionCondition, WriteHandler>* this_handler)
  {
    networking_ts_handler_invoke_helpers::invoke(
        function, this_handler->handler_);
  }

  template <typename Function, typename AsyncWriteStream,
      typename DynamicBuffer, typename CompletionCondition,
      typename WriteHandler>
  inline void networking_ts_handler_invoke(const Function& function,
      write_dynbuf_op<AsyncWriteStream, DynamicBuffer,
        CompletionCondition, WriteHandler>* this_handler)
  {
    networking_ts_handler_invoke_helpers::invoke(
        function, this_handler->handler_);
  }
} // namespace detail

#if !defined(GENERATING_DOCUMENTATION)

template <typename AsyncWriteStream, typename DynamicBuffer,
    typename CompletionCondition, typename WriteHandler, typename Allocator>
struct associated_allocator<
    detail::write_dynbuf_op<AsyncWriteStream,
      DynamicBuffer, CompletionCondition, WriteHandler>,
    Allocator>
{
  typedef typename associated_allocator<WriteHandler, Allocator>::type type;

  static type get(
      const detail::write_dynbuf_op<AsyncWriteStream,
        DynamicBuffer, CompletionCondition, WriteHandler>& h,
      const Allocator& a = Allocator()) NET_TS_NOEXCEPT
  {
    return associated_allocator<WriteHandler, Allocator>::get(h.handler_, a);
  }
};

template <typename AsyncWriteStream, typename DynamicBuffer,
    typename CompletionCondition, typename WriteHandler, typename Executor>
struct associated_executor<
    detail::write_dynbuf_op<AsyncWriteStream,
      DynamicBuffer, CompletionCondition, WriteHandler>,
    Executor>
{
  typedef typename associated_executor<WriteHandler, Executor>::type type;

  static type get(
      const detail::write_dynbuf_op<AsyncWriteStream,
        DynamicBuffer, CompletionCondition, WriteHandler>& h,
      const Executor& ex = Executor()) NET_TS_NOEXCEPT
  {
    return associated_executor<WriteHandler, Executor>::get(h.handler_, ex);
  }
};

#endif // !defined(GENERATING_DOCUMENTATION)

template <typename AsyncWriteStream,
    typename DynamicBuffer, typename WriteHandler>
inline NET_TS_INITFN_RESULT_TYPE(WriteHandler,
    void (std::error_code, std::size_t))
async_write(AsyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    NET_TS_MOVE_ARG(WriteHandler) handler,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type*)
{
  return async_write(s,
      NET_TS_MOVE_CAST(DynamicBuffer)(buffers),
      transfer_all(), NET_TS_MOVE_CAST(WriteHandler)(handler));
}

template <typename AsyncWriteStream, typename DynamicBuffer,
    typename CompletionCondition, typename WriteHandler>
inline NET_TS_INITFN_RESULT_TYPE(WriteHandler,
    void (std::error_code, std::size_t))
async_write(AsyncWriteStream& s,
    NET_TS_MOVE_ARG(DynamicBuffer) buffers,
    CompletionCondition completion_condition,
    NET_TS_MOVE_ARG(WriteHandler) handler,
    typename enable_if<
      is_dynamic_buffer<DynamicBuffer>::value
    >::type*)
{
  // If you get an error on the following line it means that your handler does
  // not meet the documented type requirements for a WriteHandler.
  NET_TS_WRITE_HANDLER_CHECK(WriteHandler, handler) type_check;

  async_completion<WriteHandler,
    void (std::error_code, std::size_t)> init(handler);

  detail::write_dynbuf_op<AsyncWriteStream,
    typename decay<DynamicBuffer>::type,
      CompletionCondition, NET_TS_HANDLER_TYPE(
        WriteHandler, void (std::error_code, std::size_t))>(
          s, NET_TS_MOVE_CAST(DynamicBuffer)(buffers),
            completion_condition, init.completion_handler)(
              std::error_code(), 0, 1);

  return init.result.get();
}

} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // NET_TS_IMPL_WRITE_HPP
