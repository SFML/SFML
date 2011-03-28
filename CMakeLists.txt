
cmake_minimum_required(VERSION 2.8)

# set a default build type if none was provided
# this has to be done before the project() instruction!
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(SFML)

# include the configuration file
include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)

# setup version numbers
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# add the SFML header path
include_directories(${CMAKE_SOURCE_DIR}/include)

# add an option for choosing the build type (shared or static)
set(BUILD_SHARED_LIBS TRUE CACHE BOOL "TRUE to build SFML as shared libraries, FALSE to build it as static libraries")

# add an option for building the examples
set(BUILD_EXAMPLES FALSE CACHE BOOL "TRUE to build the SFML examples, FALSE to ignore them")

# add an option for building the API documentation
set(BUILD_DOC FALSE CACHE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# define SFML_STATIC if the build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DSFML_STATIC)
endif()

# remove SL security warnings with Visual C++
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

# define an option for choosing between static CRT and DLL CRT (with Visual C++)
if(COMPILER_MSVC)
    set(FORCE_STATIC_VCRT FALSE CACHE BOOL "TRUE to force static VC++ runtimes, FALSE to use the DLL ones")
    if(FORCE_STATIC_VCRT)
        foreach(flag
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
endif()

# disable the rpath stuff
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# add the subdirectories
add_subdirectory(src/SFML)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(BUILD_DOC)
    add_subdirectory(doc)
endif()

# setup the install rules
install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        PATTERN ".svn" EXCLUDE)
install(FILES cmake/Modules/FindSFML.cmake DESTINATION ${CMAKE_ROOT}/Modules)
install(FILES license.txt DESTINATION ${INSTALL_MISC_DIR})
if(WINDOWS)
    if(ARCH_32BITS)
        install(FILES extlibs/bin/x86/libsndfile-1.dll DESTINATION bin)
        install(FILES extlibs/bin/x86/openal32.dll DESTINATION bin)
    else()
        install(FILES extlibs/bin/x64/libsndfile-1.dll DESTINATION bin)
        install(FILES extlibs/bin/x64/openal32.dll DESTINATION bin)
    endif()
elseif(MACOSX)
    install(DIRECTORY extlibs/libs-osx/Frameworks/sndfile.framework DESTINATION /Library/Frameworks PATTERN ".svn" EXCLUDE)
endif()
