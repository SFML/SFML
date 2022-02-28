# define the sfml-network target
sfml_add_library(Network)

# all source files
target_sources(sfml-network PRIVATE
    Ftp.cpp
    Http.cpp
    IpAddress.cpp
    Packet.cpp
    Socket.cpp
    SocketSelector.cpp
    TcpListener.cpp
    TcpSocket.cpp
    UdpSocket.cpp
)

# add platform specific sources
if(SFML_OS_WINDOWS)
    target_sources(sfml-network PRIVATE Win32/SocketImpl.cpp)
else()
    target_sources(sfml-network PRIVATE Unix/SocketImpl.cpp)
endif()

# setup dependencies
target_link_libraries(sfml-network PUBLIC SFML::System)
if(SFML_OS_WINDOWS)
    target_link_libraries(sfml-network PRIVATE ws2_32)
endif()
