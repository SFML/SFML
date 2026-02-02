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

#include <SFML/System/Time.hpp>

#include <array>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Encapsulate an IPv4 network address
///
////////////////////////////////////////////////////////////
class SFML_NETWORK_API IpAddress
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Type of IP address
    ///
    ////////////////////////////////////////////////////////////
    enum class Type
    {
        Any,  //!< Any type of address
        IpV4, //!< IPv4 address
        IpV6  //!< IPv6 address
    };

    ////////////////////////////////////////////////////////////
    /// \brief Construct the address from a null-terminated string view
    ///
    /// Here \a address can be either a decimal address
    /// (ex: "192.168.1.56") or a network name (ex: "localhost").
    ///
    /// This function will only resolve to an IPv4 address.
    /// Use Dns::resolve() to resolve to IPv6 addresses as well.
    ///
    /// \param address IP address or network name
    ///
    /// \return Address if provided argument was valid, otherwise `std::nullopt`
    ///
    ////////////////////////////////////////////////////////////
    [[deprecated("Use Dns::resolve() instead")]] [[nodiscard]] static std::optional<IpAddress> resolve(std::string_view address);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an IPv4 address from 4 bytes
    ///
    /// Calling `IpAddress(a, b, c, d)` is equivalent to calling
    /// `IpAddress::resolve("a.b.c.d")`, but safer as it doesn't
    /// have to parse a string to get the address components.
    ///
    /// \param byte0 First byte of the address
    /// \param byte1 Second byte of the address
    /// \param byte2 Third byte of the address
    /// \param byte3 Fourth byte of the address
    ///
    ////////////////////////////////////////////////////////////
    IpAddress(std::uint8_t byte0, std::uint8_t byte1, std::uint8_t byte2, std::uint8_t byte3);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an IPv4 address from a 32-bit integer
    ///
    /// This constructor uses the internal representation of
    /// the address directly. It should be used for optimization
    /// purposes, and only if you got that representation from
    /// `IpAddress::toInteger()`.
    ///
    /// \param address 4 bytes of the address packed into a 32-bit integer
    ///
    /// \see `toInteger`
    ///
    ////////////////////////////////////////////////////////////
    explicit IpAddress(std::uint32_t address);

    ////////////////////////////////////////////////////////////
    /// \brief Construct an IPv6 address from 16 bytes
    ///
    /// \param bytes Array of 16 bytes containing the address
    ///
    ////////////////////////////////////////////////////////////
    IpAddress(std::array<std::uint8_t, 16> bytes);

    ////////////////////////////////////////////////////////////
    /// \brief Try to construct an address from its string representation
    ///
    /// The string should contain either a valid representation of an
    /// IPv4 address in dotted-decimal notation or a valid representation
    /// of an IPv6 address in internet standard notation.
    ///
    /// Examples:
    ///   - 192.168.1.56
    ///   - FEDC:BA98:7654:3210:FEDC:BA98:7654:3210
    ///   - fedc:ba98:7654:3210:fedc:ba98:7654:3210
    ///   - 1080:0:0:0:8:800:200C:417A
    ///   - 1080::8:800:200C:417A
    ///   - FF01::101
    ///   - ::1
    ///   - ::
    ///   - 0:0:0:0:0:0:13.1.68.3
    ///   - ::13.1.68.3
    ///   - 0:0:0:0:0:FFFF:129.144.52.38
    ///   - ::FFFF:129.144.52.38
    ///
    /// \param address String representation of the address
    ///
    /// \return Address if provided argument was a valid string represenation of an IP address, otherwise `std::nullopt`
    ///
    /// \see `toString`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::optional<IpAddress> fromString(std::string_view address);

    ////////////////////////////////////////////////////////////
    /// \brief Get a string representation of the address
    ///
    /// The returned string is the decimal representation of the
    /// IP address (like "192.168.1.56" or "FF01::101"), even if
    /// it was constructed from a host name.
    ///
    /// \return String representation of the address
    ///
    /// \see `fromString`, `toInteger`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::string toString() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get an integer representation of the address
    ///
    /// This function can only be called if this is an IPv4
    /// address. Check with isV4() before calling this function.
    ///
    /// The returned number is the internal representation of the
    /// address, and should be used for optimization purposes only
    /// (like sending the address through a socket).
    /// The integer produced by this function can then be converted
    /// back to a `sf::IpAddress` with the proper constructor.
    ///
    /// \return 32-bits unsigned integer representation of the address
    ///
    /// \see `toString`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::uint32_t toInteger() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get an array of bytes representing the address
    ///
    /// This function can only be called if this is an IPv6
    /// address. Check with isV6() before calling this function.
    ///
    /// The returned array is the internal representation of the
    /// address, and should be used for optimization purposes only
    /// (like sending the address through a socket).
    /// The array produced by this function can then be converted
    /// back to a `sf::IpAddress` with the proper constructor.
    ///
    /// \return 16-byte array representation of the address
    ///
    /// \see `toString`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::array<std::uint8_t, 16> toBytes() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the type of this IP address
    ///
    /// \return The type of this IP address (IPv4 or IPv6)
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Type getType() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if this IP address is an IPv4 address
    ///
    /// Equivalent to getType() == Type::IPv4
    ///
    /// \return true if this is an IPv4 address, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isV4() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if this IP address is an IPv6 address
    ///
    /// Equivalent to getType() == Type::IPv6
    ///
    /// \return true if this is an IPv6 address, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isV6() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the computer's local address
    ///
    /// The local address is the address of the computer from the
    /// LAN point of view, i.e. something like 192.168.1.56. It is
    /// meaningful only for communications over the local network.
    /// Unlike getPublicAddress, this function is fast and may be
    /// used safely anywhere.
    ///
    /// \param type Type of local address
    ///
    /// \return Local IP address of the computer on success, `std::nullopt` otherwise
    ///
    /// \see `getPublicAddress`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::optional<IpAddress> getLocalAddress(Type type = Type::IpV4);

    ////////////////////////////////////////////////////////////
    /// \brief Get the computer's public address
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
    /// If tamper resistance is required, setting `secure` to `true`
    /// will make use of verified HTTPS connections to get the address.
    ///
    /// \param timeout Maximum time to wait
    /// \param type    The type of public address to get
    /// \param secure  true to retrieve the public address via a secure HTTPS connection, false to retrieve via DNS or an insecure connection
    ///
    /// \return Public IP address of the computer on success, `std::nullopt` otherwise
    ///
    /// \see `getLocalAddress`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::optional<IpAddress> getPublicAddress(Time timeout = Time::Zero,
                                                                   Type type    = Type::IpV4,
                                                                   bool secure  = false);

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    // NOLINTBEGIN(readability-identifier-naming)
    static const IpAddress Any;         //!< The same as AnyV4
    static const IpAddress LocalHost;   //!< The same as LocalHostV4
    static const IpAddress Broadcast;   //!< The same as BroadcastV4
    static const IpAddress AnyV4;       //!< Value representing any IPv4 address (0.0.0.0)
    static const IpAddress LocalHostV4; //!< The "localhost" IPv4 address (for connecting a computer to itself locally)
    static const IpAddress BroadcastV4; //!< The "broadcast" IPv4 address (for sending UDP messages to everyone on a local network)
    static const IpAddress AnyV6;       //!< Value representing any IPv6 address (::)
    static const IpAddress LocalHostV6; //!< The "localhost" IPv6 address (for connecting a computer to itself locally)
    // NOLINTEND(readability-identifier-naming)

private:
    friend SFML_NETWORK_API bool operator<(IpAddress left, IpAddress right);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    using V4Data = std::uint32_t;
    using V6Data = std::array<std::uint8_t, 16>;
    std::variant<V4Data, V6Data> m_address; //!< Address stored as an unsigned 32 bit integer or array of 16 bytes
};

////////////////////////////////////////////////////////////
/// \brief Overload of `operator==` to compare two IP addresses
///
/// \param left  Left operand (a IP address)
/// \param right Right operand (a IP address)
///
/// \return `true` if both addresses are equal
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API bool operator==(IpAddress left, IpAddress right);

////////////////////////////////////////////////////////////
/// \brief Overload of `operator!=` to compare two IP addresses
///
/// \param left  Left operand (a IP address)
/// \param right Right operand (a IP address)
///
/// \return `true` if both addresses are different
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API bool operator!=(IpAddress left, IpAddress right);

////////////////////////////////////////////////////////////
/// \brief Overload of `operator<` to compare two IP addresses
///
/// \param left  Left operand (a IP address)
/// \param right Right operand (a IP address)
///
/// \return `true` if `left` is lesser than `right`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API bool operator<(IpAddress left, IpAddress right);

////////////////////////////////////////////////////////////
/// \brief Overload of `operator>` to compare two IP addresses
///
/// \param left  Left operand (a IP address)
/// \param right Right operand (a IP address)
///
/// \return `true` if `left` is greater than `right`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API bool operator>(IpAddress left, IpAddress right);

////////////////////////////////////////////////////////////
/// \brief Overload of `operator<=` to compare two IP addresses
///
/// \param left  Left operand (a IP address)
/// \param right Right operand (a IP address)
///
/// \return `true` if \a left is lesser or equal than \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API bool operator<=(IpAddress left, IpAddress right);

////////////////////////////////////////////////////////////
/// \brief Overload of `operator>=` to compare two IP addresses
///
/// \param left  Left operand (a IP address)
/// \param right Right operand (a IP address)
///
/// \return `true` if `left` is greater or equal than `right`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_NETWORK_API bool operator>=(IpAddress left, IpAddress right);

////////////////////////////////////////////////////////////
/// \brief Overload of `operator>>` to extract an IP address from an input stream
///
/// This will only create an IP address from a string
/// representation of an IP address. In order to resolve
/// hostnames to IP addresses use the Dns::resolve() function.
///
/// \param stream  Input stream
/// \param address IP address to extract
///
/// \return Reference to the input stream
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API std::istream& operator>>(std::istream& stream, std::optional<IpAddress>& address);

////////////////////////////////////////////////////////////
/// \brief Overload of `operator<<` to print an IP address to an output stream
///
/// \param stream  Output stream
/// \param address IP address to print
///
/// \return Reference to the output stream
///
////////////////////////////////////////////////////////////
SFML_NETWORK_API std::ostream& operator<<(std::ostream& stream, IpAddress address);

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::IpAddress
/// \ingroup network
///
/// `sf::IpAddress` is a utility class for manipulating network
/// addresses. It provides a set a implicit constructors and
/// conversion functions to easily build or transform an IP
/// address from/to various representations.
///
/// Usage example:
/// \code
/// auto a0  = sf::IpAddress::fromString("127.0.0.1");                                                                  // the local host IPv4 address
/// auto a1  = sf::IpAddress::fromString("::1");                                                                        // the local host IPv6 address
/// auto a2  = sf::IpAddress::Broadcast;                                                                                // the broadcast address
/// sf::IpAddress a3(192, 168, 1, 56);                                                                                  // a local IPv4 address
/// sf::IpAddress a4({0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5d, 0x58, 0x84, 0xef, 0xc1, 0x34, 0xfd}); // a local IPv6 address
/// auto a5  = sf::IpAddress::fromString("89.54.1.169");                                                                // a distant IPv4 address
/// auto a6  = sf::IpAddress::fromString("2606:4700:4700::1111");                                                       // a distant IPv6 address
/// auto a7  = sf::IpAddress::getLocalAddress(sf::IpAddress::Type::IpV4);                                               // my IPv4 address on the local network
/// auto a8  = sf::IpAddress::getLocalAddress(sf::IpAddress::Type::IpV6);                                               // my IPv6 address on the local network
/// auto a9  = sf::IpAddress::getPublicAddress(sf::Time::Zero, sf::IpAddress::Type::IpV4);                              // my IPv4 address on the internet
/// auto a10 = sf::IpAddress::getPublicAddress(sf::Time::Zero, sf::IpAddress::Type::IpV6);                              // my IPv6 address on the internet
/// \endcode
///
/// To resolve hostnames to IP addresses, use the
/// sf::Dns::resolve() function.
///
////////////////////////////////////////////////////////////
