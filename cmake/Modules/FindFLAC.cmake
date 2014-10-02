#
# Try to find FLAC libraries and include paths.
# Once done this will define
#
# FLAC_FOUND
# FLAC_INCLUDE_DIR
# FLAC_LIBRARY
#

find_path(FLAC_INCLUDE_DIR flac/stream_decoder.h)

find_library(FLAC_LIBRARY NAMES flac)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FLAC DEFAULT_MSG FLAC_LIBRARY FLAC_INCLUDE_DIR)
