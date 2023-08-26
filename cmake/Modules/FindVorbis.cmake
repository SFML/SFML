#
# Try to find Ogg/Vorbis libraries and include paths.
# Once done this will define
#
# VORBIS_FOUND
# VORBIS_INCLUDE_DIRS
# VORBIS_LIBRARIES
#

find_path(OGG_INCLUDE_DIR ogg/ogg.h)
find_path(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h)

find_library(OGG_LIBRARY NAMES ogg)
find_library(VORBIS_LIBRARY NAMES vorbis)
find_library(VORBISFILE_LIBRARY NAMES vorbisfile)
find_library(VORBISENC_LIBRARY NAMES vorbisenc)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Vorbis DEFAULT_MSG
    OGG_INCLUDE_DIR
    OGG_LIBRARY
    VORBIS_INCLUDE_DIR
    VORBIS_LIBRARY)
mark_as_advanced(
    OGG_INCLUDE_DIR
    OGG_LIBRARY
    VORBIS_INCLUDE_DIR
    VORBIS_LIBRARY
    VORBISFILE_LIBRARY
    VORBISENC_LIBRARY)

add_library(Ogg::ogg IMPORTED UNKNOWN)
set_target_properties(Ogg::ogg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${OGG_INCLUDE_DIR})
if(OGG_LIBRARY MATCHES "/([^/]+)\\.framework$")
    set_target_properties(Ogg::ogg PROPERTIES IMPORTED_LOCATION ${OGG_LIBRARY}/${CMAKE_MATCH_1})
else()
    set_target_properties(Ogg::ogg PROPERTIES IMPORTED_LOCATION ${OGG_LIBRARY})
endif()

add_library(Vorbis::vorbis IMPORTED UNKNOWN)
set_target_properties(Vorbis::vorbis PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${VORBIS_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES Ogg::ogg)
if(VORBIS_LIBRARY MATCHES "/([^/]+)\\.framework$")
    set_target_properties(Vorbis::vorbis PROPERTIES IMPORTED_LOCATION ${VORBIS_LIBRARY}/${CMAKE_MATCH_1})
else()
    set_target_properties(Vorbis::vorbis PROPERTIES IMPORTED_LOCATION ${VORBIS_LIBRARY})
endif()

if(VORBISFILE_LIBRARY)
    add_library(Vorbis::vorbisfile IMPORTED UNKNOWN)
    set_target_properties(Vorbis::vorbisfile PROPERTIES INTERFACE_LINK_LIBRARIES Vorbis::vorbis)
    if(VORBISFILE_LIBRARY MATCHES "/([^/]+)\\.framework$")
        set_target_properties(Vorbis::vorbisfile PROPERTIES IMPORTED_LOCATION ${VORBISFILE_LIBRARY}/${CMAKE_MATCH_1})
    else()
        set_target_properties(Vorbis::vorbisfile PROPERTIES IMPORTED_LOCATION ${VORBISFILE_LIBRARY})
    endif()
endif()

if(VORBISENC_LIBRARY)
    add_library(Vorbis::vorbisenc IMPORTED UNKNOWN)
    set_target_properties(Vorbis::vorbisenc PROPERTIES INTERFACE_LINK_LIBRARIES Vorbis::vorbis)
    if(VORBISENC_LIBRARY MATCHES "/([^/]+)\\.framework$")
        set_target_properties(Vorbis::vorbisenc PROPERTIES IMPORTED_LOCATION ${VORBISENC_LIBRARY}/${CMAKE_MATCH_1})
    else()
        set_target_properties(Vorbis::vorbisenc PROPERTIES IMPORTED_LOCATION ${VORBISENC_LIBRARY})
    endif()
endif()
