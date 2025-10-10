set(NETWORK_SRC
    Ftp.test.cpp
    Http.test.cpp
    IpAddress.test.cpp
    Packet.test.cpp
    Socket.test.cpp
    SocketSelector.test.cpp
    TcpListener.test.cpp
    TcpSocket.test.cpp
    UdpSocket.test.cpp
)
sfml_add_test(test-sfml-network "${NETWORK_SRC}" SFML::Network)
