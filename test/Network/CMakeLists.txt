sfml_add_test_utils(sfml-network-test-utils
    SOURCES ../TestUtilities/NetworkUtil.hpp
            ../TestUtilities/NetworkUtil.cpp
    DEPENDS SFML::Network sfml-system-test-utils)

if(NOT SFML_OS_IOS)
    target_compile_definitions(sfml-network-test-utils PRIVATE SFML_RUN_LOOPBACK_TESTS)
endif()

sfml_set_option(SFML_RUN_IPV4_LINK_TESTS ON BOOL "ON to run tests that require an IPv4 link to be available, OFF to ignore them")
if(SFML_RUN_IPV4_LINK_TESTS)
    target_compile_definitions(sfml-network-test-utils PRIVATE SFML_RUN_IPV4_LINK_TESTS)
endif()

sfml_set_option(SFML_RUN_IPV4_INTERNET_TESTS OFF BOOL "ON to run tests that require an IPv4 route to the internet to be available, OFF to ignore them")
if(SFML_RUN_IPV4_INTERNET_TESTS)
    target_compile_definitions(sfml-network-test-utils PRIVATE SFML_RUN_IPV4_INTERNET_TESTS)
endif()

sfml_set_option(SFML_RUN_IPV6_LINK_TESTS OFF BOOL "ON to run tests that require an IPv6 link to be available, OFF to ignore them")
if(SFML_RUN_IPV6_LINK_TESTS)
    target_compile_definitions(sfml-network-test-utils PRIVATE SFML_RUN_IPV6_LINK_TESTS)
endif()

sfml_set_option(SFML_RUN_IPV6_INTERNET_TESTS OFF BOOL "ON to run tests that require an IPv6 route to the internet to be available, OFF to ignore them")
if(SFML_RUN_IPV6_INTERNET_TESTS)
    target_compile_definitions(sfml-network-test-utils PRIVATE SFML_RUN_IPV6_INTERNET_TESTS)
endif()

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
sfml_add_test(test-sfml-network SOURCES ${NETWORK_SRC} DEPENDS SFML::Network sfml-network-test-utils)

sfml_set_option(SFML_NETWORK_TESTS_MAX_FDS 256 STRING "The maximum number of open file descriptors a test should use")
target_compile_definitions(test-sfml-network PRIVATE SFML_NETWORK_TESTS_MAX_FDS=${SFML_NETWORK_TESTS_MAX_FDS})

if(SFML_RUN_IP_DISCOVERY_TESTS)
    target_compile_definitions(test-sfml-network PRIVATE SFML_RUN_IP_DISCOVERY_TESTS)
endif()
