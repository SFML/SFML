
cmake_minimum_required(VERSION 2.8)

# set a default build type if none was provided
# this has to be done before the project() instruction!
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(CSFML)

# include the configuration file
include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)

# setup version numbers
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# add the CSFML header path
include_directories(${CMAKE_SOURCE_DIR}/include)

# force dynamic build (static build is not supported)
set(BUILD_SHARED_LIBS TRUE)

# add an option for building the API documentation
set(BUILD_DOC FALSE CACHE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# disable the rpath stuff
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# add the subdirectories
add_subdirectory(src/SFML)
if(BUILD_DOC)
    add_subdirectory(doc)
endif()

# setup the install rules
install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        PATTERN ".svn" EXCLUDE)
install(FILES license.txt DESTINATION ${INSTALL_MISC_DIR})
