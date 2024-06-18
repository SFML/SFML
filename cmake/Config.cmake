# detect the OS
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(SFML_OS_WINDOWS 1)

    # don't use the OpenGL ES implementation on Windows
    set(OPENGL_ES 0)

    # detect the architecture (note: this test won't work for cross-compilation)
    include(CheckTypeSize)
    check_type_size(void* SIZEOF_VOID_PTR)

    if(${CMAKE_GENERATOR_PLATFORM} MATCHES "ARM64")
        set(ARCH_ARM64 1)
    elseif("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "" AND ${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM64")
        set(ARCH_ARM64 1)
    elseif(${SIZEOF_VOID_PTR} STREQUAL "4")
        set(ARCH_X86 1)
    elseif(${SIZEOF_VOID_PTR} STREQUAL "8")
        set(ARCH_X64 1)
    else()
        message(FATAL_ERROR "Unsupported architecture")
        return()
    endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(SFML_OS_UNIX 1)

    if(ANDROID)
        set(SFML_OS_ANDROID 1)

        # use the OpenGL ES implementation on Android
        set(OPENGL_ES 1)
    else()
        set(SFML_OS_LINUX 1)

        # don't use the OpenGL ES implementation on Linux
        set(OPENGL_ES 0)
    endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "^k?FreeBSD$")
    set(SFML_OS_FREEBSD 1)

    # don't use the OpenGL ES implementation on FreeBSD
    set(OPENGL_ES 0)
elseif(CMAKE_SYSTEM_NAME MATCHES "^OpenBSD$")
    set(SFML_OS_OPENBSD 1)

    # don't use the OpenGL ES implementation on OpenBSD
    set(OPENGL_ES 0)
elseif(CMAKE_SYSTEM_NAME MATCHES "^NetBSD$")
    set(SFML_OS_NETBSD 1)

    # don't use the OpenGL ES implementation on NetBSD
    set(OPENGL_ES 0)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    set(SFML_OS_IOS 1)
    # As we want to find packages in our extlibs folder too
    # we need to tell CMake we want to search there instead
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # use the OpenGL ES implementation on iOS
    set(OPENGL_ES 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(SFML_OS_MACOSX 1)
    # don't use the OpenGL ES implementation on Mac OS X
    set(OPENGL_ES 0)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(SFML_OS_ANDROID 1)
    # use the OpenGL ES implementation on Android
    set(OPENGL_ES 1)

# comparing CMAKE_SYSTEM_NAME with "CYGWIN" generates a false warning depending on the CMake version
# let's avoid it so the actual error is more visible
elseif(${CYGWIN})
    message(FATAL_ERROR "Unfortunately SFML doesn't support Cygwin's 'hybrid' status between both Windows and Linux derivatives.\nIf you insist on using the GCC, please use a standalone build of MinGW without the Cygwin environment instead.")
else()
    message(FATAL_ERROR "Unsupported operating system or environment")
    return()
endif()

# detect the compiler and its version
# Note: The detection is order is important because:
# - Visual Studio can both use MSVC and Clang
# - GNUCXX can still be set on macOS when using Clang
if(MSVC)
    set(SFML_COMPILER_MSVC 1)

    if(MSVC_VERSION EQUAL 1400)
        set(SFML_MSVC_VERSION 8)
    elseif(MSVC_VERSION EQUAL 1500)
        set(SFML_MSVC_VERSION 9)
    elseif(MSVC_VERSION EQUAL 1600)
        set(SFML_MSVC_VERSION 10)
    elseif(MSVC_VERSION EQUAL 1700)
        set(SFML_MSVC_VERSION 11)
    elseif(MSVC_VERSION EQUAL 1800)
        set(SFML_MSVC_VERSION 12)
    elseif(MSVC_VERSION EQUAL 1900)
        set(SFML_MSVC_VERSION 14)
    elseif(MSVC_VERSION LESS_EQUAL 1919)
        set(SFML_MSVC_VERSION 15)
    elseif(MSVC_VERSION LESS_EQUAL 1929)
        set(SFML_MSVC_VERSION 16)
    elseif(MSVC_VERSION LESS_EQUAL 1939)
        set(SFML_MSVC_VERSION 17)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(SFML_COMPILER_CLANG_CL 1)
    endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(SFML_COMPILER_CLANG 1)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
    string(REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1" SFML_CLANG_VERSION "${CLANG_VERSION_OUTPUT}")

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-v" OUTPUT_VARIABLE CLANG_COMPILER_VERSION ERROR_VARIABLE CLANG_COMPILER_VERSION)

    if("${CLANG_COMPILER_VERSION}" MATCHES "ucrt")
        set(SFML_RUNTIME_UCRT 1)
    endif()
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(SFML_COMPILER_GCC 1)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
    string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" SFML_GCC_VERSION "${GCC_VERSION_OUTPUT}")

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-v" OUTPUT_VARIABLE GCC_COMPILER_VERSION ERROR_VARIABLE GCC_COMPILER_VERSION)
    string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" SFML_COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")

    if("${GCC_COMPILER_VERSION}" MATCHES "ucrt")
        set(SFML_RUNTIME_UCRT 1)
    endif()

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpmachine" OUTPUT_VARIABLE GCC_MACHINE)
    string(STRIP "${GCC_MACHINE}" GCC_MACHINE)

    if(GCC_MACHINE MATCHES ".*w64.*")
        set(SFML_COMPILER_GCC_W64 1)
    endif()
else()
    message(WARNING "Unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}. Use at your own risk.")
endif()
