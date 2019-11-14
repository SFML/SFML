# This script provides the SFML libraries as imported targets
# ------------------------------------
#
# Usage
# -----
#
# When you try to locate the SFML libraries, you must specify which modules you want to use (system, window, graphics, network, audio, main).
# If none is given, no imported target will be created and you won't be able to link to SFML libraries.
# example:
#   find_package(SFML COMPONENTS graphics window system) # find the graphics, window and system modules
#
# You can enforce a specific version, either MAJOR.MINOR or only MAJOR.
# If nothing is specified, the version won't be checked (i.e. any version will be accepted).
# example:
#   find_package(SFML COMPONENTS ...)     # no specific version required
#   find_package(SFML 2 COMPONENTS ...)   # any 2.x version
#   find_package(SFML 2.4 COMPONENTS ...) # version 2.4 or greater
#
# By default, the dynamic libraries of SFML will be found. To find the static ones instead,
# you must set the SFML_STATIC_LIBRARIES variable to TRUE before calling find_package(SFML ...).
# You don't need to deal with SFML's dependencies when linking your targets against SFML libraries,
# they will all be configured automatically, even if you use SFML static libraries.
# example:
#   set(SFML_STATIC_LIBRARIES TRUE)
#   find_package(SFML 2 COMPONENTS network system)
#
# On macOS by default CMake will search for frameworks. If you want to use static libraries and have installed
# both SFML frameworks and SFML static libraries, your must set CMAKE_FIND_FRAMEWORK to "NEVER" or "LAST"
# in addition to setting SFML_STATIC_LIBRARIES to TRUE. Otherwise CMake will check the frameworks bundle config and
# fail after finding out that it does not provide static libraries. Please refer to CMake documentation for more details.
#
# Additionally, keep in mind that SFML frameworks are only available as release libraries unlike dylibs which
# are available for both release and debug modes.
#
# If SFML is not installed in a standard path, you can use the SFML_DIR CMake variable
# to tell CMake where SFML's config file is located (PREFIX/lib/cmake/SFML for a library-based installation,
# and PREFIX/SFML.framework/Resources/CMake on macOS for a framework-based installation).
#
# Output
# ------
#
# This script defines the following variables:
# - For each specified module XXX (system, window, graphics, network, audio, main):
#   - SFML_XXX_FOUND:  true if either the debug or release library of the xxx module is found
# - SFML_FOUND:        true if all the required modules are found
#
# And the following targets:
# - For each specified module XXX (system, window, graphics, network, audio, main):
#   - sfml-XXX
# The SFML targets are the same for both Debug and Release build configurations and will automatically provide
# correct settings based on your currently active build configuration. The SFML targets name also do not change
# when using dynamic or static SFML libraries.
#
# When linking against a SFML target, you do not need to specify indirect dependencies. For example, linking
# against sfml-graphics will also automatically link against sfml-window and sfml-system.
#
# example:
#   find_package(SFML 2 COMPONENTS graphics audio REQUIRED)
#   add_executable(myapp ...)
#   target_link_libraries(myapp sfml-graphics sfml-audio)

if (NOT SFML_FIND_COMPONENTS)
    message(FATAL_ERROR "find_package(SFML) called with no component")
endif()

set(FIND_SFML_PATHS
    "${CMAKE_CURRENT_LIST_DIR}/../.."
    ${SFML_ROOT}
    $ENV{SFML_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

find_path(SFML_DOC_DIR SFML.tag
          PATH_SUFFIXES SFML/doc share/doc/SFML
          PATHS ${FIND_SFML_PATHS})


# Update requested components (eg. request window component if graphics component was requested)
set(FIND_SFML_SYSTEM_DEPENDENCIES "")
set(FIND_SFML_MAIN_DEPENDENCIES "")
set(FIND_SFML_AUDIO_DEPENDENCIES system)
set(FIND_SFML_NETWORK_DEPENDENCIES system)
set(FIND_SFML_WINDOW_DEPENDENCIES system)
set(FIND_SFML_GRAPHICS_DEPENDENCIES window system)
set(FIND_SFML_ADDITIONAL_COMPONENTS "")
foreach(component ${SFML_FIND_COMPONENTS})
    string(TOUPPER "${component}" UPPER_COMPONENT)
    list(APPEND FIND_SFML_ADDITIONAL_COMPONENTS ${FIND_SFML_${UPPER_COMPONENT}_DEPENDENCIES})
endforeach()
list(APPEND SFML_FIND_COMPONENTS ${FIND_SFML_ADDITIONAL_COMPONENTS})
list(REMOVE_DUPLICATES SFML_FIND_COMPONENTS)

# Choose which target definitions must be imported
if (SFML_STATIC_LIBRARIES)
	set(SFML_IS_FRAMEWORK_INSTALL "@SFML_BUILD_FRAMEWORKS@")
    if (SFML_IS_FRAMEWORK_INSTALL)
        message(WARNING "Static frameworks are not supported by SFML. Clear SFML_DIR cache entry, \
and either change SFML_STATIC_LIBRARIES or CMAKE_FIND_FRAMEWORK before calling find_package(SFML)")
    endif()
    set(config_name "Static")
else()
    set(config_name "Shared")
endif()
set(targets_config_file "${CMAKE_CURRENT_LIST_DIR}/SFML${config_name}Targets.cmake")

# Generate imported targets for SFML and its dependencies
if (EXISTS "${targets_config_file}")
    # Set SFML_FOUND to TRUE by default, may be overwritten by one of the includes below
    set(SFML_FOUND TRUE)
    include("${targets_config_file}")
    include("${CMAKE_CURRENT_LIST_DIR}/SFMLConfigDependencies.cmake")

    if (SFML_FOUND)
        foreach (component ${SFML_FIND_COMPONENTS})
            string(TOUPPER "${component}" UPPER_COMPONENT)
            if (TARGET sfml-${component})
                set(SFML_${UPPER_COMPONENT}_FOUND TRUE)
            else()
                set(FIND_SFML_ERROR "Found SFML but requested component '${component}' is missing in the config defined in ${SFML_DIR}.")
                set(SFML_${UPPER_COMPONENT}_FOUND FALSE)
                set(SFML_FOUND FALSE)
            endif()
        endforeach()
    endif()
else()
    set(FIND_SFML_ERROR "Requested SFML configuration (${config_name}) was not found")
    set(SFML_FOUND FALSE)
endif()

if (NOT SFML_FOUND)
    if(SFML_FIND_REQUIRED)
        # fatal error
        message(FATAL_ERROR "${FIND_SFML_ERROR}")
    elseif(NOT SFML_FIND_QUIETLY)
        # error but continue
        message(STATUS "${FIND_SFML_ERROR}")
    endif()
endif()

if (SFML_FOUND AND NOT SFML_FIND_QUIETLY)
    message(STATUS "Found SFML @VERSION_MAJOR@.@VERSION_MINOR@.@VERSION_PATCH@ in ${CMAKE_CURRENT_LIST_DIR}")
endif()
