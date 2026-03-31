set(NETWORK_SRC
    Dns.test.cpp
    Ftp.test.cpp
    Http.test.cpp
    IpAddress.test.cpp
    Packet.test.cpp
    Sftp.test.cpp
    Socket.test.cpp
    SocketSelector.test.cpp
    TcpListener.test.cpp
    TcpLoopback.test.cpp
    TcpSocket.test.cpp
    UdpSocket.test.cpp
)
sfml_add_test(test-sfml-network "${NETWORK_SRC}" SFML::Network)

sfml_set_option(SFML_NETWORK_TESTS_MAX_FDS 256 STRING "The maximum number of open file descriptors a test should use")
target_compile_definitions(test-sfml-network PRIVATE SFML_NETWORK_TESTS_MAX_FDS=${SFML_NETWORK_TESTS_MAX_FDS})

if(SFML_RUN_IP_DISCOVERY_TESTS)
    target_compile_definitions(test-sfml-network PRIVATE SFML_RUN_IP_DISCOVERY_TESTS)
endif()
