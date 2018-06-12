//
// detail/impl/buffer_sequence_adapter.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_IMPL_BUFFER_SEQUENCE_ADAPTER_IPP
#define NET_TS_DETAIL_IMPL_BUFFER_SEQUENCE_ADAPTER_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/detail/config.hpp>

#if defined(NET_TS_WINDOWS_RUNTIME)

#include <robuffer.h>
#include <windows.storage.streams.h>
#include <wrl/implements.h>
#include <experimental/__net_ts/detail/buffer_sequence_adapter.hpp>

#include <experimental/__net_ts/detail/push_options.hpp>

namespace std {
namespace experimental {
namespace net {
inline namespace v1 {
namespace detail {

class winrt_buffer_impl :
  public Microsoft::WRL::RuntimeClass<
    Microsoft::WRL::RuntimeClassFlags<
      Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>,
    ABI::Windows::Storage::Streams::IBuffer,
    Windows::Storage::Streams::IBufferByteAccess>
{
public:
  explicit winrt_buffer_impl(const std::experimental::net::v1::const_buffer& b)
  {
    bytes_ = const_cast<byte*>(static_cast<const byte*>(b.data()));
    length_ = b.size();
    capacity_ = b.size();
  }

  explicit winrt_buffer_impl(const std::experimental::net::v1::mutable_buffer& b)
  {
    bytes_ = static_cast<byte*>(b.data());
    length_ = 0;
    capacity_ = b.size();
  }

  ~winrt_buffer_impl()
  {
  }

  STDMETHODIMP Buffer(byte** value)
  {
    *value = bytes_;
    return S_OK;
  }

  STDMETHODIMP get_Capacity(UINT32* value)
  {
    *value = capacity_;
    return S_OK;
  }

  STDMETHODIMP get_Length(UINT32 *value)
  {
    *value = length_;
    return S_OK;
  }

  STDMETHODIMP put_Length(UINT32 value)
  {
    if (value > capacity_)
      return E_INVALIDARG;
    length_ = value;
    return S_OK;
  }

private:
  byte* bytes_;
  UINT32 length_;
  UINT32 capacity_;
};

void buffer_sequence_adapter_base::init_native_buffer(
    buffer_sequence_adapter_base::native_buffer_type& buf,
    const std::experimental::net::v1::mutable_buffer& buffer)
{
  std::memset(&buf, 0, sizeof(native_buffer_type));
  Microsoft::WRL::ComPtr<IInspectable> insp
    = Microsoft::WRL::Make<winrt_buffer_impl>(buffer);
  buf = reinterpret_cast<Windows::Storage::Streams::IBuffer^>(insp.Get());
}

void buffer_sequence_adapter_base::init_native_buffer(
    buffer_sequence_adapter_base::native_buffer_type& buf,
    const std::experimental::net::v1::const_buffer& buffer)
{
  std::memset(&buf, 0, sizeof(native_buffer_type));
  Microsoft::WRL::ComPtr<IInspectable> insp
    = Microsoft::WRL::Make<winrt_buffer_impl>(buffer);
  Platform::Object^ buf_obj = reinterpret_cast<Platform::Object^>(insp.Get());
  buf = reinterpret_cast<Windows::Storage::Streams::IBuffer^>(insp.Get());
}

} // namespace detail
} // inline namespace v1
} // namespace net
} // namespace experimental
} // namespace std

#include <experimental/__net_ts/detail/pop_options.hpp>

#endif // defined(NET_TS_WINDOWS_RUNTIME)

#endif // NET_TS_DETAIL_IMPL_BUFFER_SEQUENCE_ADAPTER_IPP
