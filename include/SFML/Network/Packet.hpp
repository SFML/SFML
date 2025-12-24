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

#include <string>
#include <vector>

#include <cstddef>
#include <cstdint>


namespace sf
{
class String;

////////////////////////////////////////////////////////////
/// \brief Utility class to build blocks of data to transfer
///        over the network
///
////////////////////////////////////////////////////////////
class SFML_NETWORK_API Packet
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty packet.
    ///
    ////////////////////////////////////////////////////////////
    Packet() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Packet() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    ////////////////////////////////////////////////////////////
    Packet(const Packet&) = default;

    ////////////////////////////////////////////////////////////
    /// \brief Copy assignment
    ///
    ////////////////////////////////////////////////////////////
    Packet& operator=(const Packet&) = default;

    ////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ////////////////////////////////////////////////////////////
    Packet(Packet&&) noexcept = default;

    ////////////////////////////////////////////////////////////
    /// \brief Move assignment
    ///
    ////////////////////////////////////////////////////////////
    Packet& operator=(Packet&&) noexcept = default;

    ////////////////////////////////////////////////////////////
    /// \brief Append data to the end of the packet
    ///
    /// \param data        Pointer to the sequence of bytes to append
    /// \param sizeInBytes Number of bytes to append
    ///
    /// \see `clear`
    /// \see `getReadPosition`
    ///
    ////////////////////////////////////////////////////////////
    void append(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current reading position in the packet
    ///
    /// The next read operation will read data from this position
    ///
    /// \return The byte offset of the current read position
    ///
    /// \see `append`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::size_t getReadPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the packet
    ///
    /// After calling Clear, the packet is empty.
    ///
    /// \see `append`
    ///
    ////////////////////////////////////////////////////////////
    void clear();

    ////////////////////////////////////////////////////////////
    /// \brief Get a pointer to the data contained in the packet
    ///
    /// Warning: the returned pointer may become invalid after
    /// you append data to the packet, therefore it should never
    /// be stored.
    /// The return pointer is a `nullptr` if the packet is empty.
    ///
    /// \return Pointer to the data
    ///
    /// \see `getDataSize`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const void* getData() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the data contained in the packet
    ///
    /// This function returns the number of bytes pointed to by
    /// what `getData` returns.
    ///
    /// \return Data size, in bytes
    ///
    /// \see `getData`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::size_t getDataSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Tell if the reading position has reached the
    ///        end of the packet
    ///
    /// This function is useful to know if there is some data
    /// left to be read, without actually reading it.
    ///
    /// \return `true` if all data was read, `false` otherwise
    ///
    /// \see `operator` bool
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool endOfPacket() const;

    ////////////////////////////////////////////////////////////
    /// \brief Test the validity of the packet, for reading
    ///
    /// This operator allows to test the packet as a boolean
    /// variable, to check if a reading operation was successful.
    ///
    /// A packet will be in an invalid state if it has no more
    /// data to read.
    ///
    /// This behavior is the same as standard C++ streams.
    ///
    /// Usage example:
    /// \code
    /// float x;
    /// packet >> x;
    /// if (packet)
    /// {
    ///    // ok, x was extracted successfully
    /// }
    ///
    /// // -- or --
    ///
    /// float x;
    /// if (packet >> x)
    /// {
    ///    // ok, x was extracted successfully
    /// }
    /// \endcode
    ///
    /// \return `true` if last data extraction from packet was successful
    ///
    /// \see `endOfPacket`
    ///
    ////////////////////////////////////////////////////////////
    explicit operator bool() const;

    ////////////////////////////////////////////////////////////
    /// Overload of `operator>>` to read data from the packet
    ///
    ////////////////////////////////////////////////////////////
    Packet& operator>>(bool& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::int8_t& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::uint8_t& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::int16_t& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::uint16_t& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::int32_t& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::uint32_t& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::int64_t& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::uint64_t& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(float& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(double& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(char* data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::string& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(wchar_t* data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(std::wstring& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator>>(String& data);

    ////////////////////////////////////////////////////////////
    /// Overload of `operator<<` to write data into the packet
    ///
    ////////////////////////////////////////////////////////////
    Packet& operator<<(bool data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(std::int8_t data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(std::uint8_t data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(std::int16_t data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(std::uint16_t data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(std::int32_t data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(std::uint32_t data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(std::int64_t data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(std::uint64_t data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(float data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(double data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(const char* data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(const std::string& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(const wchar_t* data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(const std::wstring& data);

    ////////////////////////////////////////////////////////////
    /// \overload
    ////////////////////////////////////////////////////////////
    Packet& operator<<(const String& data);

protected:
    friend class TcpSocket;
    friend class UdpSocket;

    ////////////////////////////////////////////////////////////
    /// \brief Called before the packet is sent over the network
    ///
    /// This function can be defined by derived classes to
    /// transform the data before it is sent; this can be
    /// used for compression, encryption, etc.
    /// The function must return a pointer to the modified data,
    /// as well as the number of bytes pointed.
    /// The default implementation provides the packet's data
    /// without transforming it.
    ///
    /// \param size Variable to fill with the size of data to send
    ///
    /// \return Pointer to the array of bytes to send
    ///
    /// \see `onReceive`
    ///
    ////////////////////////////////////////////////////////////
    virtual const void* onSend(std::size_t& size);

    ////////////////////////////////////////////////////////////
    /// \brief Called after the packet is received over the network
    ///
    /// This function can be defined by derived classes to
    /// transform the data after it is received; this can be
    /// used for decompression, decryption, etc.
    /// The function receives a pointer to the received data,
    /// and must fill the packet with the transformed bytes.
    /// The default implementation fills the packet directly
    /// without transforming the data.
    ///
    /// \param data Pointer to the received bytes
    /// \param size Number of bytes
    ///
    /// \see `onSend`
    ///
    ////////////////////////////////////////////////////////////
    virtual void onReceive(const void* data, std::size_t size);

private:
    ////////////////////////////////////////////////////////////
    /// \brief Check if the packet can extract a given number of bytes
    ///
    /// This function updates accordingly the state of the packet.
    ///
    /// \param size Size to check
    ///
    /// \return `true` if \a size bytes can be read from the packet
    ///
    ////////////////////////////////////////////////////////////
    bool checkSize(std::size_t size);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<std::byte> m_data;          //!< Data stored in the packet
    std::size_t            m_readPos{};     //!< Current reading position in the packet
    std::size_t            m_sendPos{};     //!< Current send position in the packet (for handling partial sends)
    bool                   m_isValid{true}; //!< Reading state of the packet
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Packet
/// \ingroup network
///
/// Packets provide a safe and easy way to serialize data,
/// in order to send it over the network using sockets
/// (`sf::TcpSocket`, `sf::UdpSocket`).
///
/// Packets solve 2 fundamental problems that arise when
/// transferring data over the network:
/// \li data is interpreted correctly according to the endianness
/// \li the bounds of the packet are preserved (one send == one receive)
///
/// The `sf::Packet` class provides both input and output modes.
/// It is designed to follow the behavior of standard C++ streams,
/// using operators >> and << to extract and insert data.
///
/// It is recommended to use only fixed-size types (like `std::int32_t`, etc.),
/// to avoid possible differences between the sender and the receiver.
/// Indeed, the native C++ types may have different sizes on two platforms
/// and your data may be corrupted if that happens.
///
/// Usage example:
/// \code
/// std::uint32_t x = 24;
/// std::string s = "hello";
/// double d = 5.89;
///
/// // Group the variables to send into a packet
/// sf::Packet packet;
/// packet << x << s << d;
///
/// // Send it over the network (socket is a valid sf::TcpSocket)
/// socket.send(packet);
///
/// -----------------------------------------------------------------
///
/// // Receive the packet at the other end
/// sf::Packet packet;
/// socket.receive(packet);
///
/// // Extract the variables contained in the packet
/// std::uint32_t x;
/// std::string s;
/// double d;
/// if (packet >> x >> s >> d)
/// {
///     // Data extracted successfully...
/// }
/// \endcode
///
/// Packets have built-in `operator>>` and << overloads for
/// standard types:
/// \li `bool`
/// \li fixed-size integer types (`int[8|16|32]_t`, `uint[8|16|32]_t`)
/// \li floating point numbers (`float`, `double`)
/// \li string types (`char*`, `wchar_t*`, `std::string`, `std::wstring`, `sf::String`)
///
/// Like standard streams, it is also possible to define your own
/// overloads of operators >> and << in order to handle your
/// custom types.
///
/// \code
/// struct MyStruct
/// {
///     float       number{};
///     std::int8_t integer{};
///     std::string str;
/// };
///
/// sf::Packet& operator <<(sf::Packet& packet, const MyStruct& m)
/// {
///     return packet << m.number << m.integer << m.str;
/// }
///
/// sf::Packet& operator >>(sf::Packet& packet, MyStruct& m)
/// {
///     return packet >> m.number >> m.integer >> m.str;
/// }
/// \endcode
///
/// Packets also provide an extra feature that allows to apply
/// custom transformations to the data before it is sent,
/// and after it is received. This is typically used to
/// handle automatic compression or encryption of the data.
/// This is achieved by inheriting from `sf::Packet`, and overriding
/// the onSend and onReceive functions.
///
/// Here is an example:
/// \code
/// class ZipPacket : public sf::Packet
/// {
///     const void* onSend(std::size_t& size) override
///     {
///         const void* srcData = getData();
///         std::size_t srcSize = getDataSize();
///
///         return MySuperZipFunction(srcData, srcSize, &size);
///     }
///
///     void onReceive(const void* data, std::size_t size) override
///     {
///         std::size_t dstSize;
///         const void* dstData = MySuperUnzipFunction(data, size, &dstSize);
///
///         append(dstData, dstSize);
///     }
/// };
///
/// // Use like regular packets:
/// ZipPacket packet;
/// packet << x << s << d;
/// ...
/// \endcode
///
/// \see `sf::TcpSocket`, `sf::UdpSocket`
///
////////////////////////////////////////////////////////////
