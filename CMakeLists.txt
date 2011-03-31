
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

# Mac OS X specific options
if (MACOSX)
    # (Not supported anymore by extlibs) add an option to compile ppc/ppc64
    #set(BUILD_PPC FALSE CACHE BOOL "TRUE to build SFML for ppc and ppc64, too, FALSE to only compile i386 and x86_64")

    # add an option to build against 10.5 SDK
    set(BUILD_LEOPARD FALSE CACHE BOOL "TRUE to build SFML for OS X 10.5, FALSE to compile for default SDK")
endif()

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

# Setup Mac OS X multi arch/SDK support.
if (MACOSX)
#    # compile for PPC ?
#    if (BUILD_PPC)
#        if (NOT CMAKE_OSX_ARCHITECTURES)
#            # Custom : ppc, ppc64, i386 and x86_64
#            set(CMAKE_OSX_ARCHITECTURES "ppc;i386;ppc64;x86_64")
#        else()
#            # We got some conflict with custom user settings ; let him know his on his own.
#            message("You set BUILD_PPC to TRUE but CMAKE_OSX_ARCHITECTURES is not empty.")
#            message("You're on your own : I won't change your settings.")
#        endif()
#    else()
#        if (NOT CMAKE_OSX_ARCHITECTURES)
#            # Default : i386 and x86_64
#            set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
#        else()
#            # We got some conflict with custom user settings ; let him know his on his own.
#            message("CMAKE_OSX_ARCHITECTURES is not empty.")
#            message("You're on your own : I won't change your settings.")
#        endif()
#    endif()

    # use 10.5 SDK ?
    if (BUILD_LEOPARD)
        # Use 10.5 SDK : override default value
        set(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.5.sdk")
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.5")
    else()
        # Default SDK, let either the user or CMake decide which one to use.
    endif()
endif()

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
