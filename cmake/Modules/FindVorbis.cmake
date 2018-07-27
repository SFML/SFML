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
if (NOT SFML_OS_IOS)
    find_library(VORBISFILE_LIBRARY NAMES vorbisfile)
    find_library(VORBISENC_LIBRARY NAMES vorbisenc)
    set(VORBIS_LIBRARIES ${VORBISENC_LIBRARY} ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY} ${OGG_LIBRARY})
else()
    set(VORBIS_LIBRARIES ${VORBIS_LIBRARY} ${OGG_LIBRARY})
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(VORBIS DEFAULT_MSG VORBIS_LIBRARIES VORBIS_INCLUDE_DIR OGG_INCLUDE_DIR)

set(VORBIS_INCLUDE_DIRS ${OGG_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR})

mark_as_advanced(OGG_INCLUDE_DIR VORBIS_INCLUDE_DIR OGG_LIBRARY VORBIS_LIBRARY VORBISFILE_LIBRARY VORBISENC_LIBRARY)
