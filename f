[1mdiff --git a/include/SFML/Network/UdpSocket.hpp b/include/SFML/Network/UdpSocket.hpp[m
[1mindex 09505611..656c89b9 100644[m
[1m--- a/include/SFML/Network/UdpSocket.hpp[m
[1m+++ b/include/SFML/Network/UdpSocket.hpp[m
[36m@@ -74,6 +74,19 @@[m [mpublic:[m
     ////////////////////////////////////////////////////////////[m
     [[nodiscard]] unsigned short getLocalPort() const;[m
 [m
[32m+[m[32m    ////////////////////////////////////////////////////////////[m
[32m+[m[32m    /// \brief Get the address to which the socket is bound locally[m
[32m+[m[32m    ///[m
[32m+[m[32m    /// If the socket is not bound to an address, this function[m
[32m+[m[32m    /// returns `std::nullopt`.[m
[32m+[m[32m    ///[m
[32m+[m[32m    /// \return Address to which the socket is bound[m
[32m+[m[32m    ///[m
[32m+[m[32m    /// \see `bind`[m
[32m+[m[32m    ///[m
[32m+[m[32m    ////////////////////////////////////////////////////////////[m
[32m+[m[32m    [[nodiscard]] std::optional<IpAddress> getLocalAddress() const;[m
[32m+[m
     ////////////////////////////////////////////////////////////[m
     /// \brief Bind the socket to a specific port[m
     ///[m
[1mdiff --git a/src/SFML/Network/Http.cpp b/src/SFML/Network/Http.cpp[m
[1mindex edf66b98..6f806b1d 100644[m
[1m--- a/src/SFML/Network/Http.cpp[m
[1m+++ b/src/SFML/Network/Http.cpp[m
[36m@@ -364,8 +364,8 @@[m [mHttp::Response Http::sendRequest(const Http::Request& request, Time timeout, boo[m
 [m
     // Prepare the response[m
     Response received;[m
[31m-[m
     TcpSocket connection;[m
[32m+[m
     // Connect the socket to the host[m
     if (m_host.has_value() && connection.connect(m_host.value(), m_port, timeout) == Socket::Status::Done)[m
     {[m
[36m@@ -374,30 +374,34 @@[m [mHttp::Response Http::sendRequest(const Http::Request& request, Time timeout, boo[m
 [m
         // Convert the request to string and send it through the connected socket[m
         const std::string requestStr = toSend.prepare();[m
[31m-[m
         if (!requestStr.empty())[m
         {[m
             // Send it through the socket[m
             if (connection.send(requestStr.c_str(), requestStr.size()) == Socket::Status::Done)[m
             {[m
                 // Wait for the server's response[m
[31m-                std::string            receivedStr;[m
[31m-                std::size_t            size = 0;[m
[32m+[m[32m                std::string receivedStr;[m
[32m+[m[32m                std::size_t size = 0;[m
                 std::array<char, 1024> buffer{};[m
 [m
                 // When the HTTPS connection makes use of TLS 1.3 new session ticket[m
                 // messages can be received by the client from the server at any time[m
                 // When these messages are received the receive function will return Socket::Status::Partial[m
[31m-                // In this case We just continue to call receive until actual payload[m
[32m+[m[32m                // In this case we just continue to call receive until actual payload[m
                 // data is available, the connection is closed or an error occurs[m
                 auto result = connection.receive(buffer.data(), buffer.size(), size);[m
[31m-[m
[31m-                while ((result == Socket::Status::Done) || (result == Socket::Status::Partial))[m
[32m+[m[32m                while (result == Socket::Status::Done || result == Socket::Status::Partial)[m
                 {[m
                     // Only append payload data when it has been completely received[m
                     if (result == Socket::Status::Done)[m
[32m+[m[32m                    {[m
                         receivedStr.append(buffer.data(), buffer.data() + size);[m
 [m
[32m+[m[32m                        // Exit on clean EOF (server closed connection after sending all data)[m
[32m+[m[32m                        if (size == 0)[m
[32m+[m[32m                            break;[m
[32m+[m[32m                    }[m
[32m+[m
                     result = connection.receive(buffer.data(), buffer.size(), size);[m
                 }[m
 [m
[1mdiff --git a/src/SFML/Network/Socket.cpp b/src/SFML/Network/Socket.cpp[m
[1mindex cf5009cd..49bb4978 100644[m
[1m--- a/src/SFML/Network/Socket.cpp[m
[1m+++ b/src/SFML/Network/Socket.cpp[m
[36m@@ -121,7 +121,7 @@[m [mvoid Socket::create()[m
 ////////////////////////////////////////////////////////////[m
 void Socket::create(SocketHandle handle)[m
 {[m
[31m-    // Don't create the socket if it already exists[m
[32m+[m[32m    // If the socket already exists, just (re)apply options,  otherwise create it[m
     if (m_socket == priv::SocketImpl::invalidSocket())[m
     {[m
         // Assign the new handle[m
[1mdiff --git a/src/SFML/Network/UdpSocket.cpp b/src/SFML/Network/UdpSocket.cpp[m
[1mindex e57e55d0..b228822b 100644[m
[1m--- a/src/SFML/Network/UdpSocket.cpp[m
[1m+++ b/src/SFML/Network/UdpSocket.cpp[m
[36m@@ -135,21 +135,25 @@[m [mSocket::Status UdpSocket::send(const void* data, std::size_t size, IpAddress rem[m
 [m
 [m
 ////////////////////////////////////////////////////////////[m
[31m-Socket::Status UdpSocket::receive(void*                     data,[m
[31m-                                  std::size_t               size,[m
[31m-                                  std::size_t&              received,[m
[32m+[m[32mSocket::Status UdpSocket::receive(void* data,[m
[32m+[m[32m                                  std::size_t size,[m
[32m+[m[32m                                  std::size_t& received,[m
                                   std::optional<IpAddress>& remoteAddress,[m
[31m-                                  unsigned short&           remotePort)[m
[32m+[m[32m                                  unsigned short& remotePort)[m
 {[m
[32m+[m[32m    create();  // (si ya decidiste agregarlo, ponlo primero)[m
[32m+[m
     // First clear the variables to fill[m
[31m-    received      = 0;[m
[32m+[m[32m    received = 0;[m
     remoteAddress = std::nullopt;[m
[31m-    remotePort    = 0;[m
[32m+[m[32m    remotePort = 0;[m
 [m
[31m-    // Check the destination buffer[m
[31m-    if (!data)[m
[32m+[m[32m// Check the destination buffer and size[m
[32m+[m[32m    // Validate input parameters before attempting to receive[m
[32m+[m[32m    if (!data || size == 0)[m
     {[m
[31m-        err() << "Cannot receive data from the network (the destination buffer is invalid)" << std::endl;[m
[32m+[m[32m        err() << "Cannot receive data from the network "[m
[32m+[m[32m              << "(invalid buffer or zero size)" << std::endl;[m
         return Status::Error;[m
     }[m
 [m
