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
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Utils.hpp>

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
#include <atomic>
#include <memory>
#include <string>

#include <cassert>
#include <cstring>

using namespace std::string_literals;


namespace
{
const std::vector
    cloudflareAddresses{sf::IpAddress(1, 1, 1, 1),
                        sf::IpAddress(1, 0, 0, 1),
                        sf::IpAddress(
                            {0x26, 0x06, 0x47, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11}),
                        sf::IpAddress(
                            {0x26, 0x06, 0x47, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01})};
const std::vector
    quad9Addresses{sf::IpAddress(9, 9, 9, 9),
                   sf::IpAddress(149, 112, 112, 112),
                   sf::IpAddress(
                       {0x26, 0x20, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe}),
                   sf::IpAddress(
                       {0x26, 0x20, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09})};
const std::vector
    googleAddresses{sf::IpAddress(8, 8, 8, 8),
                    sf::IpAddress(8, 8, 4, 4),
                    sf::IpAddress(
                        {0x20, 0x01, 0x48, 0x60, 0x48, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88}),
                    sf::IpAddress(
                        {0x20, 0x01, 0x48, 0x60, 0x48, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x44})};
const std::vector
    opendnsAddresses{sf::IpAddress(208, 67, 222, 222),
                     sf::IpAddress(208, 67, 222, 220),
                     sf::IpAddress(
                         {0x26, 0x20, 0x01, 0x19, 0x00, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35}),
                     sf::IpAddress(
                         {0x26, 0x20, 0x01, 0x19, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53})};

#if defined(SFML_SYSTEM_ANDROID)
const std::vector<sf::IpAddress>& getAndroidServers()
{
    static const auto servers = []
    {
        std::vector<sf::IpAddress> theServers;
        theServers.insert(theServers.end(), cloudflareAddresses.begin(), cloudflareAddresses.end());
        theServers.insert(theServers.end(), quad9Addresses.begin(), quad9Addresses.end());
        theServers.insert(theServers.end(), googleAddresses.begin(), googleAddresses.end());
        theServers.insert(theServers.end(), opendnsAddresses.begin(), opendnsAddresses.end());
        return theServers;
    }();
    return servers;
}
#endif

#if defined(SFML_SYSTEM_WINDOWS)
struct RecordsDeleter
{
    void operator()(DNS_RECORDW* ptr) const
    {
        DnsFree(ptr, DnsFreeRecordList);
    }
};

std::unique_ptr<DNS_RECORDW, RecordsDeleter> getRecords(const std::string& hostname, WORD type)
{
    // The Win32 DnsQuery API can be a bit misleading
    // There are 3 variants of DnsQuery: DnsQuery_A, DnsQuery_W and DnsQuery_UTF8
    // Depending on which variant is used, the hostname has to be provided in the corresponding encoding
    // The problem is that they all return records in a DNS_RECORD* which when
    // UNICODE is defined is DNS_RECORDW* regardless of which variant of DnsQuery is used
    // Depending on the type of query, DNS_RECORDW can contain pointers to strings
    // These strings are encoded in the encoding specified by the variant of DnsQuery that was called
    // This means that calling DnsQuery_A would return a DNS_RECORDW* that contains wide-string pointers pointing to ASCII strings
    // Those wide-string pointers would have to be interpreted as ASCII in order to prevent data corruption
    // Because of this, the fact that UNICODE being defined forces us to use DNS_RECORDW
    // and that means that in order to not have to interpret wide-string pointers as ASCII data
    // we have to use DnsQuery_W here even though the hostname is passed as a possibly punycode encoded ASCII
    // Using DnsQuery_W ensures that the data pointed to by wide-string pointers in DNS_RECORDW is actually wide-string data
    DNS_RECORDW* recordsPtr{};
    const auto result = DnsQuery_W(sf::String(hostname).toWideString().c_str(), type, DNS_QUERY_STANDARD, nullptr, &recordsPtr, nullptr);
    std::unique_ptr<DNS_RECORDW, RecordsDeleter> records{recordsPtr};

    if (result != DNS_RCODE_NOERROR)
        return {};

    return records;
}
#else
template <typename RecordHandler>
void getRecords(const std::string& hostname, int type, const RecordHandler& recordHandler)
{
#if defined(SFML_SYSTEM_ANDROID)
    // This implementation will not work on Android due to their
    // deliberate decision to implement their own dialect of the resolv API
    // The API we use here doesn't prevent building on Android but
    // calling any of the functions will always result in an error
    // A working Android implementation would have to make use of
    // android_res_nquery() and android_res_nresult() instead
    // https://developer.android.com/ndk/reference/group/networking
    // If the user wants to perform advanced DNS queries we currently
    // have to perform it ourselves using our own manual resolver
    assert(false && "Advanced DNS queries cannot be made through res_search");
#endif

    std::array<unsigned char, NS_PACKETSZ> answer{};

    const auto length = res_search(hostname.c_str(), ns_c_in, type, answer.data(), answer.size());

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

std::uint8_t readU8(const char*& ptr, const char* end)
{
    std::uint8_t x{};
    if (ptr + 1 <= end)
    {
        std::memcpy(&x, ptr, 1);
        ptr += 1;
    }
    return x;
}

std::uint16_t readU16(const char*& ptr, const char* end)
{
    std::uint16_t x{};
    if (ptr + 2 <= end)
    {
        std::memcpy(&x, ptr, 2);
        ptr += 2;
    }
    return ntohs(x);
}

std::uint32_t readU32(const char*& ptr, const char* end)
{
    std::uint32_t x{};
    if (ptr + 4 <= end)
    {
        std::memcpy(&x, ptr, 4);
        ptr += 4;
    }
    return ntohl(x);
}

std::string readName(const char*& ptr, const char* end, const char* begin)
{
    std::string name;
    while (true)
    {
        const auto count = readU8(ptr, end);
        if (count == 0)
            break;
        if (count & 0b11000000)
        {
            // Compressed data
            const auto  offset     = (static_cast<int>(count & 0b00111111) << 8) + readU8(ptr, end);
            const auto* messagePtr = begin + offset;
            name += readName(messagePtr, end, begin);
            break;
        }
        name += std::string(ptr, count) + '.';
        ptr += count;
    }
    if (!name.empty() && (name.back() == '.'))
        name.pop_back();
    return name;
}

struct Query
{
    std::vector<sf::IpAddress> servers;
    std::uint16_t              queryType;
    std::uint16_t              queryClass;
    std::string                name;
};

template <typename Handler>
void queryDns(const Handler& handler, std::optional<sf::Time> timeout, const std::vector<Query>& queries)
{
    const auto hasIpV6 = std::any_of(queries.begin(),
                                     queries.end(),
                                     [](const auto& query) {
                                         return std::any_of(query.servers.begin(),
                                                            query.servers.end(),
                                                            [](const auto& server) { return server.isV6(); });
                                     });

    sf::UdpSocket socket;
    if (socket.bind(sf::Socket::AnyPort, hasIpV6 ? sf::IpAddress::AnyV6 : sf::IpAddress::AnyV4) != sf::Socket::Status::Done)
        return;

    static std::atomic_uint16_t nextTransactionId{1};
    const auto                  transactionId = nextTransactionId.fetch_add(1);

    const auto parseResponse = [&](const std::vector<char>& buffer)
    {
        const auto* begin = buffer.data();
        const auto* ptr   = begin;
        const auto* end   = ptr + buffer.size();

        // Verify matching transaction ID
        if (const auto responseTransactionId = readU16(ptr, end); responseTransactionId != transactionId)
            return false;

        // Ensure response and no error
        if (const auto flags = readU16(ptr, end); !(flags & 0x8000) || (flags & 0x000F))
            return false;

        const auto questions = readU16(ptr, end);
        const auto answers   = readU16(ptr, end);
        readU16(ptr, end); // Authority RRs
        readU16(ptr, end); // Additional RRs

        for (auto i = 0u; (i < questions) && (ptr < end); ++i)
        {
            readName(ptr, end, begin);
            readU16(ptr, end); // Type
            readU16(ptr, end); // Class
        }

        auto handled = false;

        for (auto i = 0u; (i < answers) && (ptr < end); ++i)
        {
            readU16(ptr, end); // Name
            const auto responseType  = readU16(ptr, end);
            const auto responseClass = readU16(ptr, end);
            readU32(ptr, end); // TTL
            const auto length = readU16(ptr, end);

            if (length == 0)
                continue;

            if (ptr + length <= end)
            {
                handler(ptr, length, begin, responseType, responseClass);
                handled = true;
            }

            ptr += length;
        }

        return handled;
    };

    auto sentQueries = 0;

    for (const auto& query : queries)
    {
        if (query.name.empty() || query.servers.empty() || (query.name.size() > 255))
            return;

        std::string queryData;
        queryData += static_cast<char>(transactionId >> 8);
        queryData += static_cast<char>(transactionId & 0xFF);
        queryData += "\1\0\0\1\0\0\0\0\0\0"s; // Standard recursive query, 1 question

        // Convert name to lowercase and append '.' if necessary
        auto lowerName = sf::toLower(query.name);

        while (!lowerName.empty() && (lowerName.back() == '.'))
            lowerName.pop_back();

        while (!lowerName.empty() && (lowerName.front() == '.'))
            lowerName.erase(0, 1);

        if (lowerName.empty())
            return;

        lowerName += '.';

        // Split name into labels and add to query
        std::string label;

        while (!lowerName.empty())
        {
            if (auto c = lowerName.front(); c == '.')
            {
                queryData += static_cast<char>(label.size());
                queryData += label;
                label.clear();
            }
            else
            {
                label += c;
                if (label.size() > 63u)
                {
                    // Label length is not allowed to exceed 63 octets
                    assert(false && "DNS hostname label length is not allowed to exceed 63 octets");
                    return;
                }
            }

            lowerName.erase(0, 1);
        }

        queryData += '\0';

        // Query type and class
        queryData += static_cast<char>(query.queryType >> 8);
        queryData += static_cast<char>(query.queryType & 0xFF);
        queryData += static_cast<char>(query.queryClass >> 8);
        queryData += static_cast<char>(query.queryClass & 0xFF);

        // Send the query to all provided servers
        // Since this only consists of sending a single UDP datagram it is relatively cheap for us
        // Once we receive the first response try to parse it and if successful return and ignore other responses
        for (const auto& server : query.servers)
        {
            if (socket.send(queryData.data(), queryData.size(), server, 53) == sf::Socket::Status::Done)
                ++sentQueries;
        }
    }

    // Check if at least one send was successful
    if (sentQueries > 0)
    {
        // At least one send was successful, now we wait for a response
        sf::SocketSelector selector;
        selector.add(socket);
        const sf::Clock clock;

        for (auto replyCount = 0; replyCount < sentQueries; ++replyCount)
        {
            if (timeout.has_value() && (clock.getElapsedTime() >= *timeout))
                break;

            if (selector.wait(timeout.has_value() ? (*timeout - clock.getElapsedTime()) : sf::Time::Zero))
            {
                std::vector<char>            buffer(1500);
                std::size_t                  received{};
                std::optional<sf::IpAddress> remote;
                std::uint16_t                port{};

                if (socket.receive(buffer.data(), buffer.size(), received, remote, port) == sf::Socket::Status::Done)
                {
                    buffer.resize(received);
                    if (parseResponse(buffer))
                        return;
                }
            }
            else
            {
                // Timed out, no need to wait for more responses
                break;
            }
        }
    }
}

std::string encodeHostname(const sf::String& hostname)
{
    // TODO: Support proper Punycode conversion if there is a need for it
    // https://datatracker.ietf.org/doc/html/rfc3492
    return hostname.toAnsiString();
}

sf::String decodeHostname(const std::string& hostname)
{
    // TODO: Support proper Punycode conversion if there is a need for it
    // https://datatracker.ietf.org/doc/html/rfc3492
    return {hostname};
}

} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
std::optional<std::vector<IpAddress>> Dns::resolve(const String&                 hostname,
                                                   const std::vector<IpAddress>& servers,
                                                   std::optional<Time>           timeout)
{
    std::vector<IpAddress> addresses;

    if (hostname.isEmpty())
        return std::nullopt;

    // Check if the hostname is actually the string representation of an IP address
    if (const auto address = IpAddress::fromString(hostname.toAnsiString()); address)
    {
        addresses.emplace_back(*address);
        return addresses;
    }

    // Not a string representation of an IP address, do a full lookup

    auto encoded = encodeHostname(hostname);

    if (!servers.empty())
    {
        // Use user provided servers
        auto querySuccess = false;

        queryDns(
            [&](const char* data, std::size_t length, const char*, std::uint16_t responseType, std::uint16_t responseClass)
            {
                querySuccess = true;

                // AAAA, IN
                if ((responseType != 28) || (responseClass != 1))
                    return;

                if (length == 16)
                {
                    std::array<std::uint8_t, 16> bytes{};
                    std::memcpy(bytes.data(), data, 16);
                    if (std::any_of(bytes.begin(), bytes.end(), [](auto b) { return b != 0; }))
                        addresses.emplace_back(bytes);
                }
            },
            timeout,
            {{servers,
              28, // AAAA
              1,  // IN
              encoded}});

        queryDns(
            [&](const char* data, std::size_t length, const char*, std::uint16_t responseType, std::uint16_t responseClass)
            {
                querySuccess = true;

                // A, IN
                if ((responseType != 1) || (responseClass != 1))
                    return;

                if (const auto bytes = readU32(data, data + length); bytes != 0)
                    addresses.emplace_back(bytes);
            },
            timeout,
            {{servers,
              1, // A
              1, // IN
              encoded}});

        // Return nullopt if we didn't get any response from the server (not even non-address responses)
        if (!querySuccess)
            return std::nullopt;

        // We got at least one response but no addresses
        return addresses;
    }

    const addrinfo hints{}; // AF_UNSPEC is the zero value for ai_family
    addrinfo*      result      = nullptr;
    const auto     queryResult = getaddrinfo(encoded.c_str(), nullptr, &hints, &result);

    if ((queryResult == 0) && (result != nullptr))
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

        return addresses;
    }

    // EAI_NODATA and EAI_NONAME indicate the server responded to the query but no addresses were returned
    // On Windows EAI_NODATA and EAI_NONAME are the same value hence the required NOLINT
    if ((queryResult == EAI_NODATA) || (queryResult == EAI_NONAME)) // NOLINT(misc-redundant-expression)
        return addresses;

    return std::nullopt;
}


////////////////////////////////////////////////////////////
std::vector<sf::String> Dns::queryNs(const String& hostname, const std::vector<IpAddress>& servers, std::optional<Time> timeout)
{
    std::vector<sf::String> nsRecords;

    if (hostname.isEmpty())
        return nsRecords;

    auto encoded = encodeHostname(hostname);

    // On Android the libc provided resolv API doesn't work, we will have to do the work ourselves
#if defined(SFML_SYSTEM_ANDROID)
    const auto& serversToQuery = servers.empty() ? getAndroidServers() : servers;
#else
    const auto& serversToQuery = servers;
#endif

    if (!serversToQuery.empty())
    {
        queryDns(
            [&](const char* data, std::size_t length, const char* messageBegin, std::uint16_t responseType, std::uint16_t responseClass)
            {
                // NS, IN
                if ((responseType != 2) || (responseClass != 1))
                    return;

                if (auto nameserver = readName(data, data + length, messageBegin); !nameserver.empty())
                    nsRecords.emplace_back(decodeHostname(nameserver));
            },
            timeout,
            {{serversToQuery,
              2, // NS
              1, // IN
              encoded}});

        return nsRecords;
    }

#if defined(SFML_SYSTEM_WINDOWS)
    const auto records = getRecords(encoded, DNS_TYPE_NS);
    for (const auto* ptr = records.get(); ptr; ptr = ptr->pNext)
    {
        if ((ptr->wType == DNS_TYPE_NS) && (ptr->Data.NS.pNameHost != nullptr))
            nsRecords.emplace_back(decodeHostname(sf::String(ptr->Data.NS.pNameHost).toAnsiString()));
    }
#else
    getRecords(encoded,
               ns_t_ns,
               [&nsRecords](const ns_msg& handle, const ns_rr& record)
               {
                   std::array<char, NS_MAXCDNAME> nameserver{};

                   if (const auto result = dn_expand(ns_msg_base(handle),
                                                     ns_msg_end(handle),
                                                     ns_rr_rdata(record),
                                                     nameserver.data(),
                                                     nameserver.size());
                       result >= 0)
                   {
                       nameserver[NS_MAXCDNAME - 1] = '\0';
                       nsRecords.emplace_back(decodeHostname(nameserver.data()));
                   }
               });
#endif

    return nsRecords;
}


////////////////////////////////////////////////////////////
std::vector<Dns::MxRecord> Dns::queryMx(const String& hostname, const std::vector<IpAddress>& servers, std::optional<Time> timeout)
{
    std::vector<MxRecord> mxRecords;

    if (hostname.isEmpty())
        return mxRecords;

    auto encoded = encodeHostname(hostname);

    // On Android the libc provided resolv API doesn't work, we will have to do the work ourselves
#if defined(SFML_SYSTEM_ANDROID)
    const auto& serversToQuery = servers.empty() ? getAndroidServers() : servers;
#else
    const auto& serversToQuery = servers;
#endif

    if (!serversToQuery.empty())
    {
        queryDns(
            [&](const char* data, std::size_t length, const char* messageBegin, std::uint16_t responseType, std::uint16_t responseClass)
            {
                // MX, IN
                if ((responseType != 15) || (responseClass != 1))
                    return;

                const auto preference = readU16(data, data + length);
                auto       exchange   = readName(data, data + length, messageBegin);

                if (!exchange.empty())
                    mxRecords.emplace_back(MxRecord{decodeHostname(exchange), preference});
            },
            timeout,
            {{serversToQuery,
              15, // MX
              1,  // IN
              encoded}});

        return mxRecords;
    }

#if defined(SFML_SYSTEM_WINDOWS)
    const auto records = getRecords(encoded, DNS_TYPE_MX);
    for (const auto* ptr = records.get(); ptr; ptr = ptr->pNext)
    {
        if ((ptr->wType == DNS_TYPE_MX) && (ptr->Data.MX.pNameExchange != nullptr))
        {
            mxRecords.emplace_back(
                MxRecord{decodeHostname(sf::String(ptr->Data.MX.pNameExchange).toAnsiString()), ptr->Data.MX.wPreference});
        }
    }
#else
    getRecords(encoded,
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
                       mxRecords.emplace_back(MxRecord{decodeHostname(exchange.data()), preference});
                   }
               });
#endif

    return mxRecords;
}


////////////////////////////////////////////////////////////
std::vector<Dns::SrvRecord> Dns::querySrv(const String& hostname, const std::vector<IpAddress>& servers, std::optional<Time> timeout)
{
    std::vector<SrvRecord> srvRecords;

    if (hostname.isEmpty())
        return srvRecords;

    auto encoded = encodeHostname(hostname);

    // On Android the libc provided resolv API doesn't work, we will have to do the work ourselves
#if defined(SFML_SYSTEM_ANDROID)
    const auto& serversToQuery = servers.empty() ? getAndroidServers() : servers;
#else
    const auto& serversToQuery = servers;
#endif

    if (!serversToQuery.empty())
    {
        queryDns(
            [&](const char* data, std::size_t length, const char* messageBegin, std::uint16_t responseType, std::uint16_t responseClass)
            {
                // SRV, IN
                if ((responseType != 33) || (responseClass != 1))
                    return;

                const auto priority = readU16(data, data + length);
                const auto weight   = readU16(data, data + length);
                const auto port     = readU16(data, data + length);
                auto       target   = readName(data, data + length, messageBegin);

                if (!target.empty())
                    srvRecords.emplace_back(SrvRecord{decodeHostname(target), port, weight, priority});
            },
            timeout,
            {{serversToQuery,
              33, // SRV
              1,  // IN
              encoded}});

        return srvRecords;
    }

#if defined(SFML_SYSTEM_WINDOWS)
    const auto records = getRecords(encoded, DNS_TYPE_SRV);
    for (const auto* ptr = records.get(); ptr; ptr = ptr->pNext)
    {
        if ((ptr->wType == DNS_TYPE_SRV) && (ptr->Data.SRV.pNameTarget != nullptr))
        {
            srvRecords.emplace_back(SrvRecord{decodeHostname(sf::String(ptr->Data.SRV.pNameTarget).toAnsiString()),
                                              ptr->Data.SRV.wPort,
                                              ptr->Data.SRV.wWeight,
                                              ptr->Data.SRV.wPriority});
        }
    }
#else
    getRecords(encoded,
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
                       srvRecords.emplace_back(SrvRecord{decodeHostname(target.data()), port, weight, priority});
                   }
               });
#endif

    return srvRecords;
}


////////////////////////////////////////////////////////////
std::vector<std::vector<sf::String>> Dns::queryTxt(const String&                 hostname,
                                                   const std::vector<IpAddress>& servers,
                                                   std::optional<Time>           timeout)
{
    std::vector<std::vector<sf::String>> txtRecords;

    if (hostname.isEmpty())
        return txtRecords;

    auto encoded = encodeHostname(hostname);

    // On Android the libc provided resolv API doesn't work, we will have to do the work ourselves
#if defined(SFML_SYSTEM_ANDROID)
    const auto& serversToQuery = servers.empty() ? getAndroidServers() : servers;
#else
    const auto& serversToQuery = servers;
#endif

    if (!serversToQuery.empty())
    {
        queryDns(
            [&](const char* data, std::size_t length, const char*, std::uint16_t responseType, std::uint16_t responseClass)
            {
                // TXT, IN
                if ((responseType != 16) || (responseClass != 1))
                    return;

                auto&             list  = txtRecords.emplace_back();
                const auto*       begin = data;
                const auto* const end   = begin + length;
                std::string       buffer;
                buffer.reserve(length);

                while (begin < end)
                {
                    buffer.clear();
                    std::uint8_t txtLength{};
                    std::memcpy(&txtLength, begin++, 1);
                    for (auto i = 0u; (i < txtLength) && (begin < end); ++i)
                        buffer.push_back(*begin++);
                    if (!buffer.empty())
                        list.emplace_back(buffer);
                }
            },
            timeout,
            {{serversToQuery,
              16, // TXT
              1,  // IN
              encoded}});

        return txtRecords;
    }

#if defined(SFML_SYSTEM_WINDOWS)
    const auto records = getRecords(encoded, DNS_TYPE_TEXT);
    for (const auto* ptr = records.get(); ptr; ptr = ptr->pNext)
    {
        if (ptr->wType == DNS_TYPE_TEXT)
        {
            auto& list = txtRecords.emplace_back();
            for (auto i = 0u; i < ptr->Data.TXT.dwStringCount; ++i)
            {
                if (ptr->Data.TXT.pStringArray[i])
                    list.emplace_back(ptr->Data.TXT.pStringArray[i]);
            }
        }
    }
#else
    getRecords(encoded,
               ns_t_txt,
               [&txtRecords](const ns_msg&, const ns_rr& record)
               {
                   auto&             list   = txtRecords.emplace_back();
                   const auto        length = static_cast<std::size_t>(ns_rr_rdlen(record));
                   const auto*       begin  = reinterpret_cast<const char*>(ns_rr_rdata(record));
                   const auto* const end    = begin + length;
                   std::string       buffer;
                   buffer.reserve(length);

                   while (begin < end)
                   {
                       buffer.clear();
                       std::uint8_t txtLength{};
                       std::memcpy(&txtLength, begin++, 1);
                       for (auto i = 0u; (i < txtLength) && (begin < end); ++i)
                           buffer.push_back(*begin++);
                       if (!buffer.empty())
                           list.emplace_back(buffer);
                   }
               });
#endif

    return txtRecords;
}


////////////////////////////////////////////////////////////
std::optional<IpAddress> Dns::getPublicAddress(std::optional<Time> timeout, IpAddress::Type type)
{
    std::optional<IpAddress> address;

    const auto handler =
        [&](const char* data, std::size_t length, const char*, std::uint16_t responseType, std::uint16_t responseClass)
    {
        // No need to handle this response if we already have an address
        if (address.has_value())
            return;

        // AAAA, IN
        if ((responseType == 28) && (responseClass == 1) && (length == 16))
        {
            std::array<std::uint8_t, 16> bytes{};
            std::memcpy(bytes.data(), data, 16);
            if (std::any_of(bytes.begin(), bytes.end(), [](auto b) { return b != 0; }))
                address.emplace(bytes);
        }

        // A, IN
        if ((responseType == 1) && (responseClass == 1))
        {
            if (const auto bytes = readU32(data, data + length); bytes != 0)
                address.emplace(bytes);
        }

        // TXT, IN or CH
        if ((responseType == 16) && ((responseClass == 1) || (responseClass == 3)))
        {
            if (const auto txtLength = readU8(data, data + length); (std::size_t{1} + txtLength) == length)
            {
                std::string str(data, txtLength);
                str.erase(std::remove(str.begin(), str.end(), '"'), str.end()); // Remove quotes if any
                if (const auto ipAddress = IpAddress::fromString(str); ipAddress)
                    address.emplace(*ipAddress);
            }
        }
    };

    const auto filter = [&type](auto filteredAddresses)
    {
        filteredAddresses.erase(std::remove_if(filteredAddresses.begin(),
                                               filteredAddresses.end(),
                                               [&type](const auto& filteredAddress)
                                               { return filteredAddress.getType() != type; }),
                                filteredAddresses.end());
        return filteredAddresses;
    };

    const auto addressQueryType = static_cast<std::uint16_t>((type == IpAddress::Type::IpV4) ? 1 : 28); // A/AAAA

    std::vector<Query> queries;

    // Attempt to query Cloudflare and OpenDNS first since those don't require their server hostnames to be looked up and resolved
    queries.emplace_back(Query{filter(cloudflareAddresses), 16, 3, "whoami.cloudflare"});           // TXT, CH
    queries.emplace_back(Query{filter(opendnsAddresses), addressQueryType, 1, "myip.opendns.com"}); // A/AAAA, IN

    queryDns(handler, timeout, queries);

    // If we already have an address no need to query Akamai and Google, return it here
    if (address.has_value())
        return address;

    queries.clear();

    static constexpr auto getAddresses = [](const char* provider)
    {
        std::vector<sf::IpAddress> providerAddresses;
        for (const auto& nameserver : queryNs(provider))
        {
            if (const auto nameserverAddresses = resolve(nameserver); nameserverAddresses)
                providerAddresses.insert(providerAddresses.end(), nameserverAddresses->begin(), nameserverAddresses->end());
        }
        return providerAddresses;
    };

    // Query Akamai and Google
    const auto akamaiNameserverAddresses = getAddresses("akamaitech.net");
    const auto googleNameserverAddresses = getAddresses("google.com");

    queries.emplace_back(Query{filter(akamaiNameserverAddresses), addressQueryType, 1, "whoami.akamai.net"}); // A/AAAA, IN
    queries.emplace_back(Query{filter(googleNameserverAddresses), 16, 1, "o-o.myaddr.l.google.com"});         // TXT, IN

    queryDns(handler, timeout, queries);

    return address;
}

} // namespace sf
