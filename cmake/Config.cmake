# detect the OS
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(SFML_OS_WINDOWS 1)

    # don't use the OpenGL ES implementation on Windows
    set(OPENGL_ES 0)

    # detect the architecture
    if("${CMAKE_GENERATOR_PLATFORM}" MATCHES "ARM64" OR "${MSVC_CXX_ARCHITECTURE_ID}" MATCHES "ARM64" OR "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "ARM64")
        set(ARCH_ARM64 1)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH_X86 1)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
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

    # use the OpenGL ES implementation on iOS
    set(OPENGL_ES 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(SFML_OS_MACOS 1)

    # don't use the OpenGL ES implementation on macOS
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

# detect the compiler
# Note: The detection is order is important because:
# - Visual Studio can both use MSVC and Clang
# - GNUCXX can still be set on macOS when using Clang
if(MSVC)
    set(SFML_COMPILER_MSVC 1)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(SFML_COMPILER_CLANG_CL 1)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(SFML_COMPILER_CLANG 1)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-v" OUTPUT_VARIABLE CLANG_COMPILER_VERSION ERROR_VARIABLE CLANG_COMPILER_VERSION)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(SFML_COMPILER_GCC 1)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-v" OUTPUT_VARIABLE GCC_COMPILER_VERSION ERROR_VARIABLE GCC_COMPILER_VERSION)
    string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" SFML_COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")
else()
    message(WARNING "Unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}. Use at your own risk.")
endif()
