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

#ifndef SFML_SOCKETTCP_HPP
#define SFML_SOCKETTCP_HPP

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
/// SocketTCP wraps a socket using TCP protocol to
/// send data safely (but a bit slower)
////////////////////////////////////////////////////////////
class SFML_API SocketTCP
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SocketTCP();

    ////////////////////////////////////////////////////////////
    /// Change the blocking state of the socket.
    /// The default behaviour of a socket is blocking
    ///
    /// \param blocking : Pass true to set the socket as blocking, or false for non-blocking
    ///
    ////////////////////////////////////////////////////////////
    void SetBlocking(bool blocking);

    ////////////////////////////////////////////////////////////
    /// Connect to another computer on a specified port
    ///
    /// \param port :    Port to use for transfers (warning : ports < 1024 are reserved)
    /// \param host :    IP Address of the host to connect to
    /// \param timeout : Maximum time to wait, in seconds (0 by default : no timeout) (this parameter is ignored for non-blocking sockets)
    ///
    /// \return True if operation has been successful
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Connect(unsigned short port, const IPAddress& host, float timeout = 0.f);

    ////////////////////////////////////////////////////////////
    /// Listen to a specified port for incoming data or connections
    ///
    /// \param port : Port to listen to
    ///
    /// \return True if operation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool Listen(unsigned short port);

    ////////////////////////////////////////////////////////////
    /// Wait for a connection (must be listening to a port).
    /// This function will block if the socket is blocking
    ///
    /// \param connected : Socket containing the connection with the connected client
    /// \param address :   Pointer to an address to fill with client infos (NULL by default)
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Accept(SocketTCP& connected, IPAddress* address = NULL);

    ////////////////////////////////////////////////////////////
    /// Send an array of bytes to the host (must be connected first)
    ///
    /// \param data : Pointer to the bytes to send
    /// \param size : Number of bytes to send
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Send(const char* data, std::size_t size);

    ////////////////////////////////////////////////////////////
    /// Receive an array of bytes from the host (must be connected first).
    /// This function will block if the socket is blocking
    ///
    /// \param data :         Pointer to a byte array to fill (make sure it is big enough)
    /// \param maxSize :      Maximum number of bytes to read
    /// \param sizeReceived : Number of bytes received
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Receive(char* data, std::size_t maxSize, std::size_t& sizeReceived);

    ////////////////////////////////////////////////////////////
    /// Send a packet of data to the host (must be connected first)
    ///
    /// \param packet : Packet to send
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Send(Packet& packet);

    ////////////////////////////////////////////////////////////
    /// Receive a packet from the host (must be connected first).
    /// This function will block if the socket is blocking
    ///
    /// \param packet : Packet to fill with received data
    ///
    /// \return Status code
    ///
    ////////////////////////////////////////////////////////////
    Socket::Status Receive(Packet& packet);

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
    /// Comparison operator ==
    ///
    /// \param other : Socket to compare
    ///
    /// \return True if *this == other
    ///
    ////////////////////////////////////////////////////////////
    bool operator ==(const SocketTCP& other) const;

    ////////////////////////////////////////////////////////////
    /// Comparison operator !=
    ///
    /// \param other : Socket to compare
    ///
    /// \return True if *this != other
    ///
    ////////////////////////////////////////////////////////////
    bool operator !=(const SocketTCP& other) const;

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
    bool operator <(const SocketTCP& other) const;

private :

    friend class Selector<SocketTCP>;

    ////////////////////////////////////////////////////////////
    /// Construct the socket from a socket descriptor
    /// (for internal use only)
    ///
    /// \param descriptor : Socket descriptor
    ///
    ////////////////////////////////////////////////////////////
    SocketTCP(SocketHelper::SocketType descriptor);

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
    SocketHelper::SocketType mySocket;            ///< Socket descriptor
    std::vector<char>        myPendingPacket;     ///< Data of the current pending packet, if any (in non-blocking mode)
    Int32                    myPendingPacketSize; ///< Size of the current pending packet, if any (in non-blocking mode)
    bool                     myIsBlocking;        ///< Is the socket blocking or non-blocking ?
};

} // namespace sf


#endif // SFML_SOCKETTCP_HPP
