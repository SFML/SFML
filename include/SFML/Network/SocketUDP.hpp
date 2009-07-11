////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_SOCKETUDP_HPP
#define SFML_SOCKETUDP_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/SocketHelper.hpp>
#include <vector>


namespace sf
{
class Packet;
class IPAddress;
template <typename> class Selector;

////////////////////////////////////////////////////////////
/// SocketUDP wraps a socket using UDP protocol to
/// send data fastly (but with less safety)
////////////////////////////////////////////////////////////
class SFML_API SocketUDP
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SocketUDP();

    ////////////////////////////////////////////////////////////
    /// Change the blocking state of the socket.
    /// The default behaviour of a socket is blocking
    ///
    /// \param blocking : Pass true to set the socket as blocking, or false for non-blocking
    ///
    ////////////////////////////////////////////////////////////
    void SetBlocking(bool blocking);

    ////////////////////////////////////////////////////////////
    /// Bind the socket to a specific port
    ///
    /// \param port : Port to bind the socket to
    ///
    /// \return True if operation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool Bind(unsigned short port);

    ////////////////////////////////////////////////////////////
    /// Unbind the socket from its previous port, if any
    ///
    /// \return True if operation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool Unbind();

    ////////////////////////////////////////////////////////////
    /// Send an array of bytes
    ///
    /// \param data :    Pointer to the bytes to send
    /// \param size :    Number of bytes to send
    /// \param address : Address of the computer to send the packet to
    /// \param port :    Port to send the data to
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Send(const char* data, std::size_t size, const IPAddress& address, unsigned short port);

    ////////////////////////////////////////////////////////////
    /// Receive an array of bytes.
    /// This function will block if the socket is blocking
    ///
    /// \param data :         Pointer to a byte array to fill (make sure it is big enough)
    /// \param maxSize :      Maximum number of bytes to read
    /// \param sizeReceived : Number of bytes received
    /// \param address :      Address of the computer which sent the data
    /// \param port :         Port on which the remote computer sent the data
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Receive(char* data, std::size_t maxSize, std::size_t& sizeReceived, IPAddress& address, unsigned short& port);

    ////////////////////////////////////////////////////////////
    /// Send a packet of data
    ///
    /// \param packet :  Packet to send
    /// \param address : Address of the computer to send the packet to
    /// \param port :    Port to send the data to
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Send(Packet& packet, const IPAddress& address, unsigned short port);

    ////////////////////////////////////////////////////////////
    /// Receive a packet.
    /// This function will block if the socket is blocking
    ///
    /// \param packet :  Packet to fill with received data
    /// \param Address : Address of the computer which sent the packet
    /// \param Port :    Port on which the remote computer sent the data
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Receive(Packet& packet, IPAddress& address, unsigned short& port);

    ////////////////////////////////////////////////////////////
    /// Close the socket
    ///
    /// \return True if operation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool Close();

    ////////////////////////////////////////////////////////////
    /// Check if the socket is in a valid state ; this function
    /// can be called any time to check if the socket is OK
    ///
    /// \return True if the socket is valid
    ///
    ////////////////////////////////////////////////////////////
    bool IsValid() const;

    ////////////////////////////////////////////////////////////
    /// Get the port the socket is currently bound to
    ///
    /// \return Current port (0 means the socket is not bound)
    ///
    ////////////////////////////////////////////////////////////
    unsigned short GetPort() const;

    ////////////////////////////////////////////////////////////
    /// Comparison operator ==
    ///
    /// \param other : Socket to compare
    ///
    /// \return True if *this == other
    ///
    ////////////////////////////////////////////////////////////
    bool operator ==(const SocketUDP& other) const;

    ////////////////////////////////////////////////////////////
    /// Comparison operator !=
    ///
    /// \param other : Socket to compare
    ///
    /// \return True if *this != other
    ///
    ////////////////////////////////////////////////////////////
    bool operator !=(const SocketUDP& other) const;

    ////////////////////////////////////////////////////////////
    /// Comparison operator <.
    /// Provided for compatibility with standard containers, as
    /// comparing two sockets doesn't make much sense...
    ///
    /// \param other : Socket to compare
    ///
    /// \return True if *this < other
    ///
    ////////////////////////////////////////////////////////////
    bool operator <(const SocketUDP& other) const;

private :

    friend class Selector<SocketUDP>;

    ////////////////////////////////////////////////////////////
    /// Construct the socket from a socket descriptor
    /// (for internal use only)
    ///
    /// \param descriptor : Socket descriptor
    ///
    ////////////////////////////////////////////////////////////
    SocketUDP(SocketHelper::SocketType descriptor);

    ////////////////////////////////////////////////////////////
    /// Create the socket
    ///
    /// \param descriptor : System socket descriptor to use (0 by default -- create a new socket)
    ///
    ////////////////////////////////////////////////////////////
    void Create(SocketHelper::SocketType descriptor = 0);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    SocketHelper::SocketType mySocket;            ///< Socket identifier
    unsigned short           myPort;              ///< Port to which the socket is bound
    std::vector<char>        myPendingPacket;     ///< Data of the current pending packet, if any (in non-blocking mode)
    Int32                    myPendingPacketSize; ///< Size of the current pending packet, if any (in non-blocking mode)
    bool                     myIsBlocking;        ///< Is the socket blocking or non-blocking ?
};

} // namespace sf


#endif // SFML_SOCKETUDP_HPP
