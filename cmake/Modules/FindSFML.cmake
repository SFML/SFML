# Locate the SFML library
#
# This module defines
#  SFML_FOUND, if false, do not try to link to SFML
#  SFML_XXX_LIBRARY, library corresponding to the XXX component (release)
#  SFML_XXX_LIBRARY_DEBUG, library corresponding to the XXX component (debug)
#  SFML_LIBRARIES, list containing all the libraries corresponding to the requested components
#  SFML_INCLUDE_DIR, where to find SFML/Config.hpp
#
# By default, the dynamic libraries of SFML will be found. To find the static ones instead,
# you must set the SFML_STATIC_LIBRARIES variable to TRUE before calling find_package(SFML ...).
#
# If SFML is not installed in a standard path, you can use the SFMLDIR CMake variable
# to tell CMake where SFML is.

# deduce the libraries suffix from the options
set(FIND_SFML_LIB_SUFFIX "")
if(SFML_STATIC_LIBRARIES)
    set(FIND_SFML_LIB_SUFFIX "${FIND_SFML_LIB_SUFFIX}-s")
endif()

# find the SFML include directory
find_path(SFML_INCLUDE_DIR SFML/Config.hpp
          PATH_SUFFIXES include
          PATHS
          ~/Library/Frameworks
          /Library/Frameworks
          /usr/local/
          /usr/
          /sw          # Fink
          /opt/local/  # DarwinPorts
          /opt/csw/    # Blastwave
          /opt/
          ${SFMLDIR})

# check the version number
set(SFML_VERSION_OK TRUE)
if(SFML_FIND_VERSION AND SFML_INCLUDE_DIR)
    # extract the major and minor version numbers from SFML/Config.hpp
    FILE(READ "${SFML_INCLUDE_DIR}/SFML/Config.hpp" SFML_CONFIG_HPP_CONTENTS)
    STRING(REGEX REPLACE ".*#define SFML_VERSION_MAJOR ([0-9]+).*" "\\1" SFML_VERSION_MAJOR "${SFML_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define SFML_VERSION_MINOR ([0-9]+).*" "\\1" SFML_VERSION_MINOR "${SFML_CONFIG_HPP_CONTENTS}")
    math(EXPR SFML_REQUESTED_VERSION "${SFML_FIND_VERSION_MAJOR} * 10 + ${SFML_FIND_VERSION_MINOR}")

    # if we could extract them, compare with the requested version number
    if (SFML_VERSION_MAJOR)
        # transform version numbers to an integer
        math(EXPR SFML_VERSION "${SFML_VERSION_MAJOR} * 10 + ${SFML_VERSION_MINOR}")

        # compare them
        if(SFML_VERSION LESS SFML_REQUESTED_VERSION)
            set(SFML_VERSION_OK FALSE)
        endif()
    else()
        # SFML version is < 2.0
        if (SFML_REQUESTED_VERSION GREATER 19)
            set(SFML_VERSION_OK FALSE)
        endif()
    endif()
endif()

# find the requested components
set(FIND_SFML_LIB_PATHS ~/Library/Frameworks
                        /Library/Frameworks
                        /usr/local
                        /usr
                        /sw
                        /opt/local
                        /opt/csw
                        /opt
                        ${SFMLDIR})
foreach(FIND_SFML_COMPONENT ${SFML_FIND_COMPONENTS})
    string(TOLOWER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_LOWER)
    string(TOUPPER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_UPPER)
    set(FIND_SFML_COMPONENT_VAR SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY)
    set(FIND_SFML_COMPONENT_NAME sfml-${FIND_SFML_COMPONENT_LOWER}${FIND_SFML_LIB_SUFFIX})

    # no suffix for sfml-main, it is always a static library
    if(FIND_SFML_COMPONENT_LOWER STREQUAL "main")
        set(FIND_SFML_COMPONENT_NAME sfml-${FIND_SFML_COMPONENT_LOWER})
    endif()

    # release library
    find_library(${FIND_SFML_COMPONENT_VAR}
                 NAMES ${FIND_SFML_COMPONENT_NAME}
                 PATH_SUFFIXES lib64 lib
                 PATHS ${FIND_SFML_LIB_PATHS})

    # debug library
    find_library(${FIND_SFML_COMPONENT_VAR}_DEBUG
                 NAMES ${FIND_SFML_COMPONENT_NAME}-d
                 PATH_SUFFIXES lib64 lib
                 PATHS ${FIND_SFML_LIB_PATHS})

    set(SFML_LIBRARIES_NAMES ${SFML_LIBRARIES_NAMES} ${FIND_SFML_COMPONENT_VAR})
    set(SFML_LIBRARIES ${SFML_LIBRARIES} ${${FIND_SFML_COMPONENT_VAR}})
endforeach()

if(SFML_FIND_REQUIRED AND NOT SFML_VERSION_OK)
    message(SEND_ERROR "Bad SFML version (requested: ${SFML_FIND_VERSION}, found: ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR})")
    set(SFML_FOUND FALSE)
else()
    # handle the QUIETLY and REQUIRED arguments and set SFML_FOUND to TRUE if all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SFML DEFAULT_MSG SFML_INCLUDE_DIR ${SFML_LIBRARIES_NAMES})
    MARK_AS_ADVANCED(SFML_INCLUDE_DIR ${SFML_LIBRARIES_NAMES})
endif()
