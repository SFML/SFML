
set(INCROOT ${CMAKE_SOURCE_DIR}/include/SFML/Network)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/SFML/Network)

# all source files
set(SRC
    ${SRCROOT}/Ftp.cpp
    ${SRCROOT}/FtpStruct.h
    ${INCROOT}/Ftp.h
    ${SRCROOT}/Http.cpp
    ${SRCROOT}/HttpStruct.h
    ${INCROOT}/Http.h
    ${SRCROOT}/IpAddress.cpp
    ${INCROOT}/IpAddress.h
    ${SRCROOT}/Packet.cpp
    ${SRCROOT}/PacketStruct.h
    ${INCROOT}/Packet.h
    ${SRCROOT}/SocketSelector.cpp
    ${SRCROOT}/SocketSelectorStruct.h
    ${INCROOT}/SocketSelector.h
    ${INCROOT}/SocketStatus.h
    ${SRCROOT}/TcpListener.cpp
    ${SRCROOT}/TcpListenerStruct.h
    ${INCROOT}/TcpListener.h
    ${SRCROOT}/TcpSocket.cpp
    ${SRCROOT}/TcpSocketStruct.h
    ${INCROOT}/TcpSocket.h
    ${INCROOT}/Types.h
    ${SRCROOT}/UdpSocket.cpp
    ${SRCROOT}/UdpSocketStruct.h
    ${INCROOT}/UdpSocket.h
)

# define the csfml-network target
csfml_add_library(csfml-network
                  SOURCES ${SRC}
                  DEPENDS optimized ${SFML_NETWORK_LIBRARY} debug ${SFML_NETWORK_LIBRARY_DEBUG}
                          optimized ${SFML_SYSTEM_LIBRARY} debug ${SFML_SYSTEM_LIBRARY_DEBUG})
