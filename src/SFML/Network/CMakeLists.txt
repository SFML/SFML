set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Network)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Network)

# all source files
set(SRC
    ${INCROOT}/Export.hpp
    ${SRCROOT}/Ftp.cpp
    ${INCROOT}/Ftp.hpp
    ${SRCROOT}/Http.cpp
    ${INCROOT}/Http.hpp
    ${SRCROOT}/IpAddress.cpp
    ${INCROOT}/IpAddress.hpp
    ${SRCROOT}/Packet.cpp
    ${INCROOT}/Packet.hpp
    ${SRCROOT}/Socket.cpp
    ${INCROOT}/Socket.hpp
    ${SRCROOT}/SocketImpl.hpp
    ${INCROOT}/SocketHandle.hpp
    ${SRCROOT}/SocketSelector.cpp
    ${INCROOT}/SocketSelector.hpp
    ${SRCROOT}/TcpListener.cpp
    ${INCROOT}/TcpListener.hpp
    ${SRCROOT}/TcpSocket.cpp
    ${INCROOT}/TcpSocket.hpp
    ${SRCROOT}/UdpSocket.cpp
    ${INCROOT}/UdpSocket.hpp
)

# add platform specific sources
if(SFML_OS_WINDOWS)
    list(APPEND SRC
        ${SRCROOT}/Win32/SocketImpl.cpp
    )
else()
    list(APPEND SRC
        ${SRCROOT}/Unix/SocketImpl.cpp
    )
endif()

source_group("" FILES ${SRC})

# define the sfml-network target
sfml_add_library(Network
                 SOURCES ${SRC}
                 DEPENDENCIES "Dependencies.cmake.in")

# setup dependencies
target_link_libraries(sfml-network PUBLIC SFML::System)
if(SFML_OS_WINDOWS)
    target_link_libraries(sfml-network PRIVATE Crypt32 ws2_32)
endif()

if(SFML_USE_SYSTEM_DEPS)
    find_package(MbedTLS REQUIRED)
else()
    # use an immediately invoked function to scope option variables we have to set
    function(sfml_add_network_dependencies)
        include(FetchContent)

        # remember whether we are building SFML as a shared library
        if(BUILD_SHARED_LIBS)
            set(SFML_BUILD_SHARED_LIBS ON)
        endif()

        set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
        set(BUILD_SHARED_LIBS OFF)
        set(ENABLE_TESTING OFF)
        set(MBEDTLS_AS_SUBPROJECT OFF)
        set(ENABLE_PROGRAMS OFF)
        set(MBEDTLS_FATAL_WARNINGS OFF)
        set(GEN_FILES OFF)

        FetchContent_Declare(MbedTLS
            GIT_REPOSITORY https://github.com/Mbed-TLS/mbedtls.git
            GIT_TAG v3.6.4
            GIT_SHALLOW ON
            # patch out parts we don't want of the MbedTLS CMake configuration and write to configuration files
            # - disable building 3rdparty targets
            # - add CMAKE_DEBUG_POSTFIX
            # - configure mbedtls_config.h
            # - provide threading_alt.h
            PATCH_COMMAND ${CMAKE_COMMAND} -DMBEDTLS_DIR=${FETCHCONTENT_BASE_DIR}/mbedtls-src -P ${PROJECT_SOURCE_DIR}/tools/mbedtls/PatchMbedTLS.cmake)
        FetchContent_MakeAvailable(MbedTLS)

        set_target_properties(mbedtls mbedcrypto mbedx509 PROPERTIES FOLDER "Dependencies")

        # if building SFML as a shared library and linking our dependencies in
        # as static libraries we need to build them with -fPIC
        if(SFML_BUILD_SHARED_LIBS)
            set_target_properties(mbedtls mbedcrypto mbedx509 PROPERTIES POSITION_INDEPENDENT_CODE ON)
        endif()

        # disable building dependencies as part of a unity build, they don't support it
        set_target_properties(mbedtls mbedcrypto mbedx509 PROPERTIES UNITY_BUILD OFF)

        sfml_set_stdlib(mbedtls)
        sfml_set_stdlib(mbedcrypto)
        sfml_set_stdlib(mbedx509)
    endfunction()
    sfml_add_network_dependencies()
endif()

target_link_libraries(sfml-network PRIVATE mbedtls mbedcrypto mbedx509)

if(SFML_OS_MACOS)
    target_link_libraries(sfml-network PRIVATE "-framework Security" "-framework CoreFoundation")
endif()
