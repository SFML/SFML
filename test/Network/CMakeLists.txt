set(NETWORK_SRC
    Dns.test.cpp
    Ftp.test.cpp
    Http.test.cpp
    IpAddress.test.cpp
    Packet.test.cpp
    Socket.test.cpp
    SocketSelector.test.cpp
    TcpListener.test.cpp
    TcpLoopback.test.cpp
    TcpSocket.test.cpp
    UdpSocket.test.cpp
)
sfml_add_test(test-sfml-network "${NETWORK_SRC}" SFML::Network)
if(SFML_RUN_IP_DISCOVERY_TESTS)
    target_compile_definitions(test-sfml-network PRIVATE SFML_RUN_IP_DISCOVERY_TESTS)
endif()
