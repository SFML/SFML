
# detect the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    # FreeBSD compile path is the same as Linux
    set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX 1)

    # detect OS X version. (use '/usr/bin/sw_vers -productVersion' to extract V from '10.V.x'.)
    EXEC_PROGRAM(/usr/bin/sw_vers ARGS -productVersion OUTPUT_VARIABLE MACOSX_VERSION_RAW)
    STRING(REGEX REPLACE "10\\.([0-9]).*" "\\1" MACOSX_VERSION "${MACOSX_VERSION_RAW}")
    if(${MACOSX_VERSION} LESS 5)
        message(WARNING "Unsupported version of OS X : ${MACOSX_VERSION_RAW}")
        return()
    endif()

else()
    message(WARNING "Unsupported operating system")
    return()
endif()

# detect the architecture
# note: this test won't work for cross-compilation
include(CheckTypeSize)
check_type_size(void* SIZEOF_VOID_PTR)
if(${SIZEOF_VOID_PTR} MATCHES "^4$")
    set(ARCH_32BITS 1)
else()
    set(ARCH_64BITS 1)
endif()

# detect the compiler and its version
if(CMAKE_COMPILER_IS_GNUCXX)
    set(COMPILER_GCC 1)
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
    string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" GCC_VERSION "${GCC_VERSION_OUTPUT}")
elseif(MSVC_VERSION EQUAL 1400)
    set(COMPILER_MSVC 2005)
elseif(MSVC_VERSION EQUAL 1500)
    set(COMPILER_MSVC 2008)
elseif(MSVC_VERSION EQUAL 1600)
    set(COMPILER_MSVC 2010)
else()
    message(WARNING "Unsupported compiler")
    return()
endif()

# define the install directory for miscellaneous files
if(WINDOWS)
    set(INSTALL_MISC_DIR .)
elseif(UNIX)
    set(INSTALL_MISC_DIR share/SFML)
endif()
