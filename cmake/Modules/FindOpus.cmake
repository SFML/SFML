#
# Try to find the Opus libraries and include paths
# OPUS_FOUND
# OPUS_INCLUDE_DIR
# OPUS_LIBRARY
#

# find the opusfile header, defines our api.
find_path(OPUS_INCLUDE_DIR
	NAMES opus/opusfile.h
	DOC "Opus include directory"
)
mark_as_advanced(OPUS_INCLUDE_DIR)

# look for libopusfile, the highlevel container-aware api.
find_library(OPUSFILE_LIBRARY
	NAMES opusfile
	DOC "Path to OpusFile library"
)
mark_as_advanced(OPUSFILE_LIBRARY)

# find libopus, the core codec component.
find_library(OPUS_LIBRARY
	NAMES opus
	DOC "Path to Opus library"
)
mark_as_advanced(OPUS_LIBRARY)

add_library(Opus::opus IMPORTED UNKNOWN)
set_target_properties(Opus::opus PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${OPUS_INCLUDE_DIR})
if(OPUS_LIBRARY MATCHES "/([^/]+)\\.framework$")
    set_target_properties(Opus::opus PROPERTIES IMPORTED_LOCATION ${OPUS_LIBRARY}/${CMAKE_MATCH_1})
else()
    set_target_properties(Opus::opus PROPERTIES IMPORTED_LOCATION ${OPUS_LIBRARY})
endif()
