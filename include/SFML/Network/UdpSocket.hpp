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
#include <SFML/Network/Socket.hpp>

#include <optional>
#include <vector>

#include <cstddef>


namespace sf
{
class Packet;

////////////////////////////////////////////////////////////
/// \brief Specialized socket using the UDP protocol
///
////////////////////////////////////////////////////////////
class SFML_NETWORK_API UdpSocket : public Socket
{
public:
    ////////////////////////////////////////////////////////////
    // Constants
    ////////////////////////////////////////////////////////////
    // NOLINTNEXTLINE(readability-identifier-naming)
    static constexpr std::size_t MaxDatagramSize{65507}; //!< The maximum number of bytes that can be sent in a single UDP datagram

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    UdpSocket();

    ////////////////////////////////////////////////////////////
    /// \brief Get the port to which the socket is bound locally
    ///
    /// If the socket is not bound to a port, this function
    /// returns 0.
    ///
    /// \return Port to which the socket is bound
    ///
    /// \see `bind`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] unsigned short getLocalPort() const;

    ////////////////////////////////////////////////////////////
    /// \brief Bind the socket to a specific port
    ///
    /// Binding the socket to a port is necessary for being
    /// able to receive data on that port.
    ///
    /// When providing `sf::Socket::AnyPort` as port, the listener
    /// will request an available port from the system.
    /// The chosen port can be retrieved by calling `getLocalPort()`.
    ///
    /// Since the socket can only be bound to a single port at
    /// any given moment, if it is already bound when this
    /// function is called, it will be unbound from the previous
    /// port before being bound to the new one.
    ///
    /// \param port    Port to bind the socket to
    /// \param address Address of the interface to bind to
    ///
    /// \return Status code
    ///
    /// \see `unbind`, `getLocalPort`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Status bind(unsigned short port, IpAddress address = IpAddress::Any);

    ////////////////////////////////////////////////////////////
    /// \brief Unbind the socket from the local port to which it is bound
    ///
    /// The port that the socket was previously bound to is immediately
    /// made available to the operating system after this function is called.
    /// This means that a subsequent call to `bind()` will be able to re-bind
    /// the port if no other process has done so in the mean time.
    /// If the socket is not bound to a port, this function has no effect.
    ///
    /// \see `bind`
    ///
    ////////////////////////////////////////////////////////////
    void unbind();

    ////////////////////////////////////////////////////////////
    /// \brief Send raw data to a remote peer
    ///
    /// Make sure that `size` is not greater than
    /// `UdpSocket::MaxDatagramSize`, otherwise this function will
    /// fail and no data will be sent.
    ///
    /// \param data          Pointer to the sequence of bytes to send
    /// \param size          Number of bytes to send
    /// \param remoteAddress Address of the receiver
    /// \param remotePort    Port of the receiver to send the data to
    ///
    /// \return Status code
    ///
    /// \see `receive`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Status send(const void* data, std::size_t size, IpAddress remoteAddress, unsigned short remotePort);

    ////////////////////////////////////////////////////////////
    /// \brief Receive raw data from a remote peer
    ///
    /// In blocking mode, this function will wait until some
    /// bytes are actually received.
    /// Be careful to use a buffer which is large enough for
    /// the data that you intend to receive, if it is too small
    /// then an error will be returned and *all* the data will
    /// be lost.
    ///
    /// \param data          Pointer to the array to fill with the received bytes
    /// \param size          Maximum number of bytes that can be received
    /// \param received      This variable is filled with the actual number of bytes received
    /// \param remoteAddress Address of the peer that sent the data
    /// \param remotePort    Port of the peer that sent the data
    ///
    /// \return Status code
    ///
    /// \see `send`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Status receive(void*                     data,
                                 std::size_t               size,
                                 std::size_t&              received,
                                 std::optional<IpAddress>& remoteAddress,
                                 unsigned short&           remotePort);

    ////////////////////////////////////////////////////////////
    /// \brief Send a formatted packet of data to a remote peer
    ///
    /// Make sure that the packet size is not greater than
    /// `UdpSocket::MaxDatagramSize`, otherwise this function will
    /// fail and no data will be sent.
    ///
    /// \param packet        Packet to send
    /// \param remoteAddress Address of the receiver
    /// \param remotePort    Port of the receiver to send the data to
    ///
    /// \return Status code
    ///
    /// \see `receive`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Status send(Packet& packet, IpAddress remoteAddress, unsigned short remotePort);

    ////////////////////////////////////////////////////////////
    /// \brief Receive a formatted packet of data from a remote peer
    ///
    /// In blocking mode, this function will wait until the whole packet
    /// has been received.
    ///
    /// \param packet        Packet to fill with the received data
    /// \param remoteAddress Address of the peer that sent the data
    /// \param remotePort    Port of the peer that sent the data
    ///
    /// \return Status code
    ///
    /// \see `send`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Status receive(Packet& packet, std::optional<IpAddress>& remoteAddress, unsigned short& remotePort);

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<std::byte> m_buffer{MaxDatagramSize}; //!< Temporary buffer holding the received data in Receive(Packet)
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::UdpSocket
/// \ingroup network
///
/// A UDP socket is a connectionless socket. Instead of
/// connecting once to a remote host, like TCP sockets,
/// it can send to and receive from any host at any time.
///
/// It is a datagram protocol: bounded blocks of data (datagrams)
/// are transferred over the network rather than a continuous
/// stream of data (TCP). Therefore, one call to send will always
/// match one call to receive (if the datagram is not lost),
/// with the same data that was sent.
///
/// The UDP protocol is lightweight but unreliable. Unreliable
/// means that datagrams may be duplicated, be lost or
/// arrive reordered. However, if a datagram arrives, its
/// data is guaranteed to be valid.
///
/// UDP is generally used for real-time communication
/// (audio or video streaming, real-time games, etc.) where
/// speed is crucial and lost data doesn't matter much.
///
/// Sending and receiving data can use either the low-level
/// or the high-level functions. The low-level functions
/// process a raw sequence of bytes, whereas the high-level
/// interface uses packets (see `sf::Packet`), which are easier
/// to use and provide more safety regarding the data that is
/// exchanged. You can look at the `sf::Packet` class to get
/// more details about how they work.
///
/// It is important to note that `UdpSocket` is unable to send
/// datagrams bigger than `MaxDatagramSize`. In this case, it
/// returns an error and doesn't send anything. This applies
/// to both raw data and packets. Indeed, even packets are
/// unable to split and recompose data, due to the unreliability
/// of the protocol (dropped, mixed or duplicated datagrams may
/// lead to a big mess when trying to recompose a packet).
///
/// If the socket is bound to a port, it is automatically
/// unbound from it when the socket is destroyed. However,
/// you can unbind the socket explicitly with the Unbind
/// function if necessary, to stop receiving messages or
/// make the port available for other sockets.
///
/// Usage example:
/// \code
/// // ----- The client -----
///
/// // Create a socket and bind it to the port 55001
/// sf::UdpSocket socket;
/// socket.bind(55001);
///
/// // Send a message to 192.168.1.50 on port 55002
/// std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
/// socket.send(message.c_str(), message.size() + 1, "192.168.1.50", 55002);
///
/// // Receive an answer (most likely from 192.168.1.50, but could be anyone else)
/// std::array<char, 1024> buffer;
/// std::size_t received = 0;
/// std::optional<sf::IpAddress> sender;
/// unsigned short port;
/// if (socket.receive(buffer.data(), buffer.size(), received, sender, port) == sf::Socket::Status::Done)
///     std::cout << sender->toString() << " said: " << buffer.data() << std::endl;
///
/// // ----- The server -----
///
/// // Create a socket and bind it to the port 55002
/// sf::UdpSocket socket;
/// socket.bind(55002);
///
/// // Receive a message from anyone
/// std::array<char, 1024> buffer;
/// std::size_t received = 0;
/// std::optional<sf::IpAddress> sender;
/// unsigned short port;
/// if (socket.receive(buffer.data(), buffer.size(), received, sender, port) == sf::Socket::Status::Done)
///     std::cout << sender->toString() << " said: " << buffer.data() << std::endl;
///
/// // Send an answer
/// std::string message = "Welcome " + sender.toString();
/// socket.send(message.c_str(), message.size() + 1, sender, port);
/// \endcode
///
/// \see `sf::Socket`, `sf::TcpSocket`, `sf::Packet`
///
////////////////////////////////////////////////////////////
