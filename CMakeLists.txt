
cmake_minimum_required(VERSION 2.8)

# set a default build type if none was provided
# this has to be done before the project() instruction!
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(SFML)

# include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# setup version numbers
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# add the SFML header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add an option for choosing the build type (shared or static)
set(BUILD_SHARED_LIBS TRUE CACHE BOOL "TRUE to build SFML as shared libraries, FALSE to build it as static libraries")

# add an option for building the examples
set(BUILD_EXAMPLES FALSE CACHE BOOL "TRUE to build the SFML examples, FALSE to ignore them")

# add an option for building the API documentation
set(BUILD_DOC FALSE CACHE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# Mac OS X specific options
if(MACOSX)
    # add an option to build frameworks instead of dylibs (release only)
    set(BUILD_FRAMEWORKS FALSE CACHE BOOL "TRUE to build SFML as frameworks libraries (release only), FALSE to build according to BUILD_SHARED_LIBS")
    
    # add an option to let the user specify a custom directory for frameworks installation (SFML, sndfile, ...)
    set(CMAKE_INSTALL_FRAMEWORK_PREFIX "/Library/Frameworks" CACHE STRING "Frameworks installation directory")

    # add an option to automatically install Xcode 4 templates
    set(INSTALL_XCODE4_TEMPLATES FALSE CACHE BOOL "TRUE to automatically install the Xcode 4 templates, FALSE to do nothing about it")
endif()

# define SFML_STATIC if the build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DSFML_STATIC)
endif()

# remove SL security warnings with Visual C++
if(COMPILER_MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

# define an option for choosing between static and dynamic C runtime (Windows only)
if(WINDOWS)
    set(STATIC_STD_LIBS FALSE CACHE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs")
    
    # for VC++, we can apply it globally by modifying the compiler flags
    if(COMPILER_MSVC AND STATIC_STD_LIBS)
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

# Setup Mac OS X stuff
if(MACOSX)
    # multi arch support - by default : i386 and x86_64
    if(NOT CMAKE_OSX_ARCHITECTURES)
        set(CMAKE_OSX_ARCHITECTURES "i386;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
    endif()

    # multi SDK support - by default we choose the older SDK available starting by 10.5 SDK
    if(NOT OSX_CONFIG_HAS_BEEN_RUN_BEFORE)
        if(EXISTS /Developer/SDKs/MacOSX10.5.sdk)
            # target 10.5 system
            set(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.5.sdk"
                CACHE STRING "The product will be built against the headers and libraries located inside the indicated SDK. Set to empty string for default value."
                FORCE)
            set(CMAKE_OSX_DEPLOYMENT_TARGET "10.5" 
                CACHE STRING "Minimum OS X version to target for deployment (at runtime); ewer APIs weak linked."
                FORCE)
        elseif(EXISTS /Developer/SDKs/MacOSX10.6.sdk)
            # target 10.6 system
            set(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.6.sdk"
                CACHE STRING "The product will be built against the headers and libraries located inside the indicated SDK. Set to empty string for default value."
                FORCE)
            set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6" 
                CACHE STRING "Minimum OS X version to target for deployment (at runtime); newer APIs weak linked."
                FORCE)
        else()
            # use default SDK.
        endif()

        # note : we use OSX_CONFIG_HAS_BEEN_RUN_BEFORE to be able to let the user set his/her custom settings
        #        so we don't always have to FORCE the value of CMAKE_OSX_DEPLOYMENT_TARGET and CMAKE_OSX_SYSROOT
        set(OSX_CONFIG_HAS_BEEN_RUN_BEFORE TRUE
            CACHE BOOL "Don't edit this value; you should instead empty your cache."
            FORCE)
        mark_as_advanced(OSX_CONFIG_HAS_BEEN_RUN_BEFORE)
    endif()

    # BUILD_FRAMEWORKS needs two things :
    # first, it's available only for release
    #    (because cmake currently doesn't allow specifying a custom framework name so XXX-d is not possible)
    # secondly, it works only with BUILD_SHARED_LIBS enabled
    if(BUILD_FRAMEWORKS)
        # requirement #1
        if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
            message(WARNING "CMAKE_BUILD_TYPE should be \"Release\" when BUILD_FRAMEWORKS is TRUE")
            return()
        endif()

        # requirement #2
        if(NOT BUILD_SHARED_LIBS)
            message(WARNING "BUILD_SHARED_LIBS should be TRUE when BUILD_FRAMEWORKS is TRUE")
            return()
        endif()
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
if(NOT BUILD_FRAMEWORKS)
    install(DIRECTORY include
            DESTINATION .
            COMPONENT devel
            PATTERN ".svn" EXCLUDE)
else()
    # find only "root" headers
    file(GLOB SFML_HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "include/SFML/*")

    # in fact we have to fool cmake to copy all the headers in subdirectories
    # to do that we have to add the "root" headers to the PUBLIC_HEADER
    # then we can run a post script to copy the remaining headers

    # we need a dummy file in order to compile the framework
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp
                       COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

    set(SFML_SOURCES ${SFML_HEADERS})
    list(APPEND SFML_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

    # create SFML.framework
    add_library(SFML ${SFML_SOURCES})

    # edit target properties
    set_target_properties(SFML PROPERTIES 
                          FRAMEWORK TRUE
                          FRAMEWORK_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          MACOSX_FRAMEWORK_IDENTIFIER org.sfml-dev.SFML
                          MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          PUBLIC_HEADER "${SFML_HEADERS}")

    # add the remaining headers
    add_custom_command(TARGET SFML 
                       POST_BUILD
                       COMMAND cp -r ${PROJECT_SOURCE_DIR}/include/SFML/* SFML.framework/Versions/2.0.0/Headers)

    # adapt install directory to allow distributing dylibs/frameworks in user’s frameworks/application bundle
    # NOTE : it's not required to link agains SFML.framework
    set_target_properties(SFML PROPERTIES 
                          BUILD_WITH_INSTALL_RPATH 1 
                          INSTALL_NAME_DIR "@executable_path/../Frameworks")

    # install rule
    install(TARGETS SFML
            FRAMEWORK DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX}
            COMPONENT devel)
endif()

install(FILES cmake/Modules/FindSFML.cmake DESTINATION ${INSTALL_MISC_DIR}/cmake/Modules)
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
    install(DIRECTORY extlibs/libs-osx/Frameworks/sndfile.framework DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX})

    if(INSTALL_XCODE4_TEMPLATES)
        install(DIRECTORY xcode/templates/SFML DESTINATION $ENV{HOME}/Library/Developer/Xcode/Templates)
    endif()
endif()
