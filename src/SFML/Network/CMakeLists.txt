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
    ${SRCROOT}/Sftp.cpp
    ${INCROOT}/Sftp.hpp
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
    find_package(Libssh2 REQUIRED)
else()
    include(FetchContent)

    # use a block to scope option variables we have to set
    block()
        set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
        set(BUILD_SHARED_LIBS OFF)
        set(ENABLE_TESTING OFF)
        set(MBEDTLS_AS_SUBPROJECT OFF)
        set(ENABLE_PROGRAMS OFF)
        set(MBEDTLS_FATAL_WARNINGS OFF)
        set(GEN_FILES OFF)

        FetchContent_Declare(MbedTLS
            URL https://github.com/Mbed-TLS/mbedtls/releases/download/mbedtls-3.6.5/mbedtls-3.6.5.tar.bz2
            URL_HASH SHA256=4a11f1777bb95bf4ad96721cac945a26e04bf19f57d905f241fe77ebeddf46d8
            DOWNLOAD_NO_PROGRESS ON
            OVERRIDE_FIND_PACKAGE
            # patch out parts we don't want of the MbedTLS CMake configuration and write to configuration files
            # - disable building 3rdparty targets
            # - add CMAKE_DEBUG_POSTFIX
            # - configure mbedtls_config.h
            # - provide threading_alt.h
            PATCH_COMMAND ${CMAKE_COMMAND} -DMBEDTLS_DIR=${FETCHCONTENT_BASE_DIR}/mbedtls-src -P ${PROJECT_SOURCE_DIR}/tools/mbedtls/PatchMbedTLS.cmake)
        FetchContent_MakeAvailable(MbedTLS)
    endblock()

    # use a block to scope option variables we have to set
    block()
        set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
        set(BUILD_SHARED_LIBS OFF)
        set(BUILD_EXAMPLES OFF)
        set(BUILD_TESTING OFF)
        set(CRYPTO_BACKEND "mbedTLS" CACHE STRING "")
        set(LIBSSH2_NO_DEPRECATED ON)
        set(LIBSSH2_BUILD_DOCS OFF)

        FetchContent_Declare(libssh2
            URL https://github.com/libssh2/libssh2/archive/704299e997bf518375dc9222670c57b800ac59e6.tar.gz
            URL_HASH SHA256=10c3a28bf3099b2a4a6c21a7f4a180fb1bacf5ee6560ccc5542e62f762b2f4f1
            DOWNLOAD_NO_PROGRESS ON
            # patch broken parts of libssh2
            # - Fix returning a pointer to stack memory by forcing libssh to make a copy of it instead
            PATCH_COMMAND ${CMAKE_COMMAND} -DLIBSSH2_DIR=${FETCHCONTENT_BASE_DIR}/libssh2-src -DMODULES_DIR=${CMAKE_CURRENT_LIST_DIR}/../../../cmake/Modules -P ${PROJECT_SOURCE_DIR}/tools/libssh2/PatchLibssh2.cmake)
        FetchContent_MakeAvailable(libssh2)
    endblock()

    set_target_properties(mbedtls mbedcrypto mbedx509 libssh2_static PROPERTIES FOLDER "Dependencies" SYSTEM ON)

    # if building SFML as a shared library or on android (where exes are shared libs) and linking our dependencies in
    # as static libraries we need to build them with -fPIC
    if(BUILD_SHARED_LIBS OR SFML_OS_ANDROID)
        set_target_properties(mbedtls mbedcrypto mbedx509 libssh2_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()

    # disable building dependencies as part of a unity build, they don't support it
    set_target_properties(mbedtls mbedcrypto mbedx509 libssh2_static PROPERTIES UNITY_BUILD OFF)

    sfml_set_stdlib(mbedtls)
    sfml_set_stdlib(mbedcrypto)
    sfml_set_stdlib(mbedx509)
    sfml_set_stdlib(libssh2_static)

    # Set libssh2 to include/link to MbedTLS
    target_compile_definitions(libssh2_static PRIVATE LIBSSH2_MBEDTLS)
    target_include_directories(libssh2_static PRIVATE "${FETCHCONTENT_BASE_DIR}/mbedtls-src/include")

    # Disable all warnings
    target_compile_options(libssh2_static PRIVATE -w)
endif()

target_link_libraries(sfml-network PRIVATE libssh2::libssh2 mbedtls mbedcrypto mbedx509)

if(SFML_OS_WINDOWS)
    target_include_directories(sfml-network SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/wepoll")
endif()

if(SFML_OS_MACOS)
    target_link_libraries(sfml-network PRIVATE "-framework Security" "-framework CoreFoundation")
endif()
