
# detect the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX 1)
else()
    message(WARNING "Unsupported operating system")
    return()
endif()

# detect the architecture
# note: this test won't work for cross-compilation
include(CheckTypeSize)
check_type_size(void* SIZEOF_VOID_PTR)
if(${SIZEOF_VOID_PTR} MATCHES "^8$")
    set(ARCH_BITS 64)
else()
    set(ARCH_BITS 32)
endif()

# detect the compiler and its version
if(CMAKE_COMPILER_IS_GNUCXX)
    set(COMPILER_GCC 1)
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
    string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" GCC_VERSION "${GCC_VERSION_OUTPUT}")
elseif(MSVC_VERSION EQUAL 1400)
    set(COMPILER_MSVC 1)
    set(MSVC_VERSION 2005)
elseif(MSVC_VERSION EQUAL 1500)
    set(COMPILER_MSVC 1)
    set(MSVC_VERSION 2008)
elseif(MSVC_VERSION EQUAL 1600)
    set(COMPILER_MSVC 1)
    set(MSVC_VERSION 2010)
else()
    message(WARNING "Unsupported compiler")
    return()
endif()

# define the install directory for miscellaneous files
if(WINDOWS)
    set(INSTALL_MISC_DIR .)
elseif(UNIX)
    set(INSTALL_MISC_DIR share/CSFML)
endif()
