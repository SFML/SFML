////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Dns.hpp>
#include <SFML/Network/SocketImpl.hpp>

#include <SFML/System/Utf.hpp>

#if defined(SFML_SYSTEM_WINDOWS)
#include <windns.h>
#else
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netdb.h>
#include <resolv.h>
#endif

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <cassert>
#include <cstdint>
#include <cstring>


namespace
{
#if defined(SFML_SYSTEM_WINDOWS)
struct RecordsDeleter
{
    void operator()(DNS_RECORDW* ptr) const
    {
        DnsFree(ptr, DnsFreeRecordList);
    }
};

std::unique_ptr<DNS_RECORDW, RecordsDeleter> getRecords(const sf::String& hostname, WORD type)
{
    const auto   wide = hostname.toWideString();
    DNS_RECORDW* recordsPtr{};
    const auto   result = DnsQuery_W(wide.c_str(), type, DNS_QUERY_STANDARD, nullptr, &recordsPtr, nullptr);
    std::unique_ptr<DNS_RECORDW, RecordsDeleter> records{recordsPtr};

    if (result != DNS_RCODE_NOERROR)
        return {};

    return records;
}
#else
template <typename RecordHandler>
void getRecords(const sf::String& hostname, int type, const RecordHandler& recordHandler)
{
#if defined(SFML_SYSTEM_ANDROID)
    // This implementation will not work on Android due to their
    // deliberate decision to implement their own dialect of the resolv API
    // The API we use here doesn't prevent building on Android but
    // calling any of the functions will always result in an error
    // A working Android implementation would have to make use of
    // android_res_nquery() and android_res_nresult() instead
    // https://developer.android.com/ndk/reference/group/networking
    assert(false && "Advanced DNS queries not currently implemented on Android");
#endif

    const auto                             utf8 = hostname.toUtf8();
    std::array<unsigned char, NS_PACKETSZ> answer{};

    const auto length = res_search(reinterpret_cast<const char*>(utf8.c_str()), ns_c_in, type, answer.data(), answer.size());

    if (length < 0)
        return;

    ns_msg handle{};
    ns_initparse(answer.data(), length, &handle);

    for (int i = 0; i < ns_msg_count(handle, ns_s_an); ++i)
    {
        ns_rr record{};

        if (const auto result = ns_parserr(&handle, ns_s_an, i, &record); result < 0)
            continue;

        if (const auto recordType = ns_rr_type(record); recordType != type)
            continue;

        recordHandler(handle, record);
    }
}
#endif
} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
std::vector<IpAddress> Dns::resolve(const sf::String& hostname)
{
    std::vector<IpAddress> addresses;

    if (hostname.isEmpty())
        return addresses;

    // Check if the hostname is actually the string representation of an IP address
    if (const auto address = IpAddress::fromString(hostname.toAnsiString()); address)
    {
        addresses.emplace_back(*address);
        return addresses;
    }

    // Not a string representation of an IP address, do a full lookup
    const addrinfo hints{}; // AF_UNSPEC is the zero value for ai_family

    if (addrinfo* result = nullptr;
        (getaddrinfo(hostname.toAnsiString().c_str(), nullptr, &hints, &result) == 0) && (result != nullptr))
    {
        for (auto* info = result; info; info = info->ai_next)
        {
            if (info->ai_family == AF_INET6)
            {
                // IPv6
                sockaddr_in6 sin{};
                std::memcpy(&sin, info->ai_addr, sizeof(sin));
                const std::array<std::uint8_t, 16>
                    bytes{sin.sin6_addr.s6_addr[0],
                          sin.sin6_addr.s6_addr[1],
                          sin.sin6_addr.s6_addr[2],
                          sin.sin6_addr.s6_addr[3],
                          sin.sin6_addr.s6_addr[4],
                          sin.sin6_addr.s6_addr[5],
                          sin.sin6_addr.s6_addr[6],
                          sin.sin6_addr.s6_addr[7],
                          sin.sin6_addr.s6_addr[8],
                          sin.sin6_addr.s6_addr[9],
                          sin.sin6_addr.s6_addr[10],
                          sin.sin6_addr.s6_addr[11],
                          sin.sin6_addr.s6_addr[12],
                          sin.sin6_addr.s6_addr[13],
                          sin.sin6_addr.s6_addr[14],
                          sin.sin6_addr.s6_addr[15]};
                addresses.emplace_back(bytes);
            }
            else if (info->ai_family == AF_INET)
            {
                // IPv4
                sockaddr_in sin{};
                std::memcpy(&sin, info->ai_addr, sizeof(sin));
                addresses.emplace_back(ntohl(sin.sin_addr.s_addr));
            }
        }

        freeaddrinfo(result);
    }

    return addresses;
}


////////////////////////////////////////////////////////////
std::vector<Dns::MxRecord> Dns::queryMX(const sf::String& hostname)
{
    std::vector<MxRecord> mxRecords;

    if (hostname.isEmpty())
        return mxRecords;

#if defined(SFML_SYSTEM_WINDOWS)
    const auto records = getRecords(hostname, DNS_TYPE_MX);
    for (const auto* ptr = records.get(); ptr; ptr = ptr->pNext)
    {
        if ((ptr->wType == DNS_TYPE_MX) && (ptr->Data.MX.pNameExchange != nullptr))
        {
            mxRecords.emplace_back(
                MxRecord{reinterpret_cast<const char*>(ptr->Data.MX.pNameExchange), ptr->Data.MX.wPreference});
        }
    }
#else
    getRecords(hostname,
               ns_t_mx,
               [&mxRecords](const ns_msg& handle, const ns_rr& record)
               {
                   const auto preference = static_cast<std::uint16_t>(ns_get16(ns_rr_rdata(record) + 0 * NS_INT16SZ));

                   std::array<char, NS_MAXCDNAME> exchange{};

                   if (const auto result = dn_expand(ns_msg_base(handle),
                                                     ns_msg_end(handle),
                                                     ns_rr_rdata(record) + 1 * NS_INT16SZ,
                                                     exchange.data(),
                                                     exchange.size());
                       result >= 0)
                   {
                       exchange[NS_MAXCDNAME - 1] = '\0';
                       mxRecords.emplace_back(MxRecord{exchange.data(), preference});
                   }
               });
#endif

    return mxRecords;
}


////////////////////////////////////////////////////////////
std::vector<Dns::SrvRecord> Dns::querySRV(const sf::String& hostname)
{
    std::vector<SrvRecord> srvRecords;

    if (hostname.isEmpty())
        return srvRecords;

#if defined(SFML_SYSTEM_WINDOWS)
    const auto records = getRecords(hostname, DNS_TYPE_SRV);
    for (const auto* ptr = records.get(); ptr; ptr = ptr->pNext)
    {
        if ((ptr->wType == DNS_TYPE_SRV) && (ptr->Data.SRV.pNameTarget != nullptr))
        {
            srvRecords.emplace_back(SrvRecord{reinterpret_cast<const char*>(ptr->Data.SRV.pNameTarget),
                                              ptr->Data.SRV.wPort,
                                              ptr->Data.SRV.wWeight,
                                              ptr->Data.SRV.wPriority});
        }
    }
#else
    getRecords(hostname,
               ns_t_srv,
               [&srvRecords](const ns_msg& handle, const ns_rr& record)
               {
                   const auto priority = static_cast<std::uint16_t>(ns_get16(ns_rr_rdata(record) + 0 * NS_INT16SZ));
                   const auto weight   = static_cast<std::uint16_t>(ns_get16(ns_rr_rdata(record) + 1 * NS_INT16SZ));
                   const auto port     = static_cast<std::uint16_t>(ns_get16(ns_rr_rdata(record) + 2 * NS_INT16SZ));

                   std::array<char, NS_MAXCDNAME> target{};

                   if (const auto result = dn_expand(ns_msg_base(handle),
                                                     ns_msg_end(handle),
                                                     ns_rr_rdata(record) + 3 * NS_INT16SZ,
                                                     target.data(),
                                                     target.size());
                       result >= 0)
                   {
                       target[NS_MAXCDNAME - 1] = '\0';
                       srvRecords.emplace_back(SrvRecord{target.data(), port, weight, priority});
                   }
               });
#endif

    return srvRecords;
}


////////////////////////////////////////////////////////////
std::vector<std::string> Dns::queryTXT(const sf::String& hostname)
{
    std::vector<std::string> txtRecords;

    if (hostname.isEmpty())
        return txtRecords;

#if defined(SFML_SYSTEM_WINDOWS)
    const auto records = getRecords(hostname, DNS_TYPE_TEXT);
    for (const auto* ptr = records.get(); ptr; ptr = ptr->pNext)
    {
        if ((ptr->wType == DNS_TYPE_TEXT) && (ptr->Data.TXT.dwStringCount == 1) &&
            (ptr->Data.TXT.pStringArray[0] != nullptr))
            txtRecords.emplace_back(reinterpret_cast<const char*>(ptr->Data.TXT.pStringArray[0]));
    }
#else
    getRecords(hostname,
               ns_t_txt,
               [&txtRecords](const ns_msg& handle, const ns_rr& record)
               {
                   if (const auto length = std::min(static_cast<std::size_t>(*(ns_rr_rdata(record) + 0 * NS_INT8SZ)),
                                                    static_cast<std::size_t>(ns_msg_size(handle) - NS_INT8SZ));
                       length > 0)
                   {
                       std::vector<char> buffer(length);
                       std::memcpy(buffer.data(), ns_rr_rdata(record) + 1 * NS_INT8SZ, length);
                       txtRecords.emplace_back(buffer.data(), length);
                   }
               });
#endif

    return txtRecords;
}

} // namespace sf
