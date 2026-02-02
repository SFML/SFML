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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Export.hpp>

#include <SFML/Network/IpAddress.hpp>

#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

#include <cstdint>


////////////////////////////////////////////////////////////
/// \brief Perform Domain Name System queries to lookup
///        DNS records for various purposes
///
////////////////////////////////////////////////////////////
namespace sf::Dns
{
////////////////////////////////////////////////////////////
/// \brief Resolve a hostname into a list of IP addresses
///
/// \param hostname Hostname to resolve
/// \param servers  The list of servers to query, if empty use the default servers
/// \param timeout  Query timeout if using a provided list of servers
///
/// \return List of IP addresses the given hostname resolves to, an empty list is returned if name resolution fails
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API std::vector<IpAddress> resolve(const sf::String&                 hostname,
                                                              const std::vector<sf::IpAddress>& servers = {},
                                                              sf::Time timeout = sf::seconds(1));

////////////////////////////////////////////////////////////
/// \brief Query NS records for a hostname
///
/// \param hostname Hostname to query NS records for
/// \param servers  The list of servers to query, if empty use the default servers
/// \param timeout  Query timeout if using a provided list of servers
///
/// \return List of NS record strings, an empty list is returned if there are no NS records for the hostname
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API std::vector<sf::String> queryNS(const sf::String&                 hostname,
                                                               const std::vector<sf::IpAddress>& servers = {},
                                                               sf::Time timeout = sf::seconds(1));

////////////////////////////////////////////////////////////
/// \brief A DNS MX record
///
////////////////////////////////////////////////////////////
struct MxRecord
{
    sf::String    exchange;     //!< Host willing to act as mail exchange
    std::uint16_t preference{}; //!< Preference of this record among others, lower values are preferred
};

////////////////////////////////////////////////////////////
/// \brief Query MX records for a hostname
///
/// \param hostname Hostname to query MX records for
/// \param servers  The list of servers to query, if empty use the default servers
/// \param timeout  Query timeout if using a provided list of servers
///
/// \return List of MX records, an empty list is returned if there are no MX records for the hostname
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API std::vector<MxRecord> queryMX(const sf::String&                 hostname,
                                                             const std::vector<sf::IpAddress>& servers = {},
                                                             sf::Time timeout = sf::seconds(1));

////////////////////////////////////////////////////////////
/// \brief A DNS SRV record
///
////////////////////////////////////////////////////////////
struct SrvRecord
{
    sf::String    target; //!< The domain name of the target host
    std::uint16_t port{}; //!< The port on the target host of the service
    std::uint16_t weight{}; //!< Server selection mechanism, larger weights should be given a proportionately higher probability of being selected
    std::uint16_t priority{}; //!< The priority of the target host, a client must attempt to contact the target host with the lowest-numbered priority it can reach
};

////////////////////////////////////////////////////////////
/// \brief Query SRV records for a hostname
///
/// \param hostname Hostname to query SRV records for
/// \param servers  The list of servers to query, if empty use the default servers
/// \param timeout  Query timeout if using a provided list of servers
///
/// \return List of SRV records, an empty list is returned if there are no SRV records for the hostname
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API std::vector<SrvRecord> querySRV(const sf::String&                 hostname,
                                                               const std::vector<sf::IpAddress>& servers = {},
                                                               sf::Time timeout = sf::seconds(1));

////////////////////////////////////////////////////////////
/// \brief Query TXT records for a hostname
///
/// \param hostname Hostname to query TXT records for
/// \param servers  The list of servers to query, if empty use the default servers
/// \param timeout  Query timeout if using a provided list of servers
///
/// \return List of TXT record strings, an empty list is returned if there are no TXT records for the hostname
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API std::vector<sf::String> queryTXT(const sf::String&                 hostname,
                                                                const std::vector<sf::IpAddress>& servers = {},
                                                                sf::Time timeout = sf::seconds(1));

////////////////////////////////////////////////////////////
/// \brief Get the computer's public address via DNS
///
/// The public address is the address of the computer from the
/// point of view of the internet, i.e. something like 89.54.1.169
/// or 2600:1901:0:13e0::1 as opposed to a private or local address
/// like 192.168.1.56 or fe80::1234:5678:9abc.
/// It is necessary for communication with hosts outside of the
/// local network.
///
/// The only way to reliably get the public address is to send
/// data to a host on the internet and see what the origin
/// address is; as a consequence, this function depends on both
/// your network connection and the server, and may be very slow.
/// You should try to use it as little as possible. Because this
/// function depends on the network connection and on a distant
/// server, you can specify a time limit if you don't want your
/// program to get stuck waiting in case there is a problem; this
/// limit is deactivated by default.
///
/// This function makes use of DNS queries get the public address.
///
/// \param timeout Maximum time to wait
/// \param type    The type of public address to get
///
/// \return Public IP address of the computer on success, `std::nullopt` otherwise
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API std::optional<IpAddress> getPublicAddress(Time            timeout = Time::Zero,
                                                                         IpAddress::Type type = IpAddress::Type::IpV4);
} // namespace sf::Dns


////////////////////////////////////////////////////////////
/// \namespace sf::Dns
/// \ingroup network
///
/// The `sf::Dns` functions allow querying the Domain Name System.
/// The Domain Name System contains many different kinds of
/// records. The most common records are A and AAAA records
/// used to resolve hostnames to IPv4 and IPv6 addresses
/// respectively. Additionally, other kinds of records such
/// as MX, SRV and TXT records can be queried.
///
/// Usage example:
/// \code
/// auto addresses0 = sf::Dns::resolve("127.0.0.1");            // the local host address
/// auto addresses1 = sf::Dns::resolve("my_computer");          // a list of local addresses created from a network name
/// auto addresses2 = sf::Dns::resolve("89.54.1.169");          // a distant IPv4 address
/// auto addresses3 = sf::Dns::resolve("2606:4700:4700::1111"); // a distant IPv6 address
/// auto addresses4 = sf::Dns::resolve("www.google.com");       // a list of distant address created from a network name
///
/// for (const auto& address : addresses4)
/// {
///     if (address.isV4())
///     {
///         // Do something with IPv4 address
///     }
///     else
///     {
///         // Do something with IPv6 address
///     }
/// }
///
/// auto nsRecords  = sf::Dns::queryNS("sfml-dev.org");
/// auto mxRecords  = sf::Dns::queryMX("sfml-dev.org");
/// auto txtRecords = sf::Dns::queryTXT("sfml-dev.org");
/// auto srvRecords = sf::Dns::querySRV("_xmpp-client._tcp.xmpp.org");
/// \endcode
///
////////////////////////////////////////////////////////////
