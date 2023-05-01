# detect the OS
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(SFML_OS_WINDOWS 1)

    # don't use the OpenGL ES implementation on Windows
    set(OPENGL_ES 0)

    # detect the architecture (note: this test won't work for cross-compilation)
    include(CheckTypeSize)
    check_type_size(void* SIZEOF_VOID_PTR)
    if(${SIZEOF_VOID_PTR} STREQUAL "4")
        set(ARCH_32BITS 1)
    elseif(${SIZEOF_VOID_PTR} STREQUAL "8")
        set(ARCH_64BITS 1)
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
    # we need to tell CMake we want to search there instead
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

# set pkgconfig install directory
# this could be e.g. macports on mac or msys2 on windows etc.
set(SFML_PKGCONFIG_DIR "/${CMAKE_INSTALL_LIBDIR}/pkgconfig")

if(SFML_OS_FREEBSD OR SFML_OS_OPENBSD OR SFML_OS_NETBSD)
    set(SFML_PKGCONFIG_DIR "/libdata/pkgconfig")
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
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(SFML_COMPILER_GCC 1)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE GCC_COMPILER_VERSION)
    string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" SFML_COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")
else()
    message(FATAL_ERROR "Unsupported compiler")
    return()
endif()
