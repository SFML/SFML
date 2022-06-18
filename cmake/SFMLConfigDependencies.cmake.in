# in case of static linking, we must also define the list of all the dependencies of SFML libraries
if(SFML_STATIC_LIBRARIES)
    # detect the OS
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(FIND_SFML_OS_WINDOWS 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(FIND_SFML_OS_LINUX 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
        set(FIND_SFML_OS_FREEBSD 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        if (DEFINED IOS)
            set(FIND_SFML_OS_IOS 1)
        else()
            set(FIND_SFML_OS_MACOSX 1)
        endif()
    endif()

    # start with an empty list
    set(FIND_SFML_DEPENDENCIES_NOTFOUND)

    # macro that searches for a 3rd-party library
    function(sfml_bind_dependency)
        cmake_parse_arguments(THIS "" "TARGET;FRIENDLY_NAME" "SEARCH_NAMES" ${ARGN})
        if (THIS_UNPARSED_ARGUMENTS)
            message(FATAL_ERROR "Unknown arguments when calling sfml_bind_dependency: ${THIS_UNPARSED_ARGUMENTS}")
        endif()

        # No lookup in environment variables (PATH on Windows), as they may contain wrong library versions
        find_library(${THIS_FRIENDLY_NAME}_LIB NAMES ${THIS_SEARCH_NAMES}
                     PATHS ${FIND_SFML_PATHS} PATH_SUFFIXES lib NO_SYSTEM_ENVIRONMENT_PATH)
        mark_as_advanced(${THIS_FRIENDLY_NAME}_LIB)
        if(${THIS_FRIENDLY_NAME}_LIB)
            set_property(TARGET ${THIS_TARGET} APPEND PROPERTY INTERFACE_LINK_LIBRARIES "${${THIS_FRIENDLY_NAME}_LIB}")
        else()
            set(FIND_SFML_DEPENDENCIES_NOTFOUND "${FIND_SFML_DEPENDENCIES_NOTFOUND} ${THIS_FRIENDLY_NAME}" PARENT_SCOPE)
        endif()
    endfunction()

    # SFML::Window
    list(FIND SFML_FIND_COMPONENTS "Window" FIND_SFML_WINDOW_COMPONENT_INDEX)
    if(FIND_SFML_WINDOW_COMPONENT_INDEX GREATER -1)
        if(FIND_SFML_OS_LINUX OR FIND_SFML_OS_FREEBSD)
            sfml_bind_dependency(TARGET X11 FRIENDLY_NAME "X11" SEARCH_NAMES "X11")
            sfml_bind_dependency(TARGET X11 FRIENDLY_NAME "Xrandr" SEARCH_NAMES "Xrandr")
            sfml_bind_dependency(TARGET X11 FRIENDLY_NAME "Xcursor" SEARCH_NAMES "Xcursor")
        endif()

        if(FIND_SFML_OS_LINUX)
            sfml_bind_dependency(TARGET UDev FRIENDLY_NAME "UDev" SEARCH_NAMES "udev" "libudev")
        endif()

        if (FIND_SFML_OS_WINDOWS)
            set_property(TARGET OpenGL APPEND PROPERTY INTERFACE_LINK_LIBRARIES "OpenGL32")
        elseif(NOT FIND_SFML_OS_IOS)
            sfml_bind_dependency(TARGET OpenGL FRIENDLY_NAME "OpenGL" SEARCH_NAMES "OpenGL" "GL")
        endif()
    endif()

    # SFML::Graphics
    list(FIND SFML_FIND_COMPONENTS "Graphics" FIND_SFML_GRAPHICS_COMPONENT_INDEX)
    if(FIND_SFML_GRAPHICS_COMPONENT_INDEX GREATER -1)
        sfml_bind_dependency(TARGET Freetype FRIENDLY_NAME "FreeType" SEARCH_NAMES "freetype")
    endif()

    # SFML::Audio
    list(FIND SFML_FIND_COMPONENTS "Audio" FIND_SFML_AUDIO_COMPONENT_INDEX)
    if(FIND_SFML_AUDIO_COMPONENT_INDEX GREATER -1)
        sfml_bind_dependency(TARGET OpenAL FRIENDLY_NAME "OpenAL" SEARCH_NAMES "OpenAL" "openal" "openal32")
        if (NOT FIND_SFML_OS_IOS)
            sfml_bind_dependency(TARGET VORBIS FRIENDLY_NAME "VorbisFile" SEARCH_NAMES "vorbisfile")
            sfml_bind_dependency(TARGET VORBIS FRIENDLY_NAME "VorbisEnc" SEARCH_NAMES "vorbisenc")
        endif()
        sfml_bind_dependency(TARGET VORBIS FRIENDLY_NAME "Vorbis" SEARCH_NAMES "vorbis")
        sfml_bind_dependency(TARGET VORBIS FRIENDLY_NAME "Ogg" SEARCH_NAMES "ogg")
        sfml_bind_dependency(TARGET FLAC FRIENDLY_NAME "FLAC" SEARCH_NAMES "FLAC")
    endif()

    if (FIND_SFML_DEPENDENCIES_NOTFOUND)
        set(FIND_SFML_ERROR "SFML found but some of its dependencies are missing (${FIND_SFML_DEPENDENCIES_NOTFOUND})")
        set(SFML_FOUND FALSE)
    endif()
endif()
