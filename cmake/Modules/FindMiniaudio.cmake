#
# Try to find Miniaudio libraries and include paths.
# Once done this will define
#
# MINIAUDIO_FOUND
# MINIAUDIO_INCLUDE_DIR
# MINIAUDIO_LIBRARY
#

find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_MINIAUDIO miniaudio)
endif()

find_library(MINIAUDIO_LIBRARY
    NAMES miniaudio
    HINTS ${PC_MINIAUDIO_LIBRARY_DIRS}
)
find_path(MINIAUDIO_INCLUDE_DIR
    NAMES miniaudio/miniaudio.h
    HINTS ${PC_MINIAUDIO_INCLUDE_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Miniaudio DEFAULT_MSG MINIAUDIO_LIBRARY MINIAUDIO_INCLUDE_DIR)

add_library(Miniaudio::Miniaudio IMPORTED UNKNOWN)
set_target_properties(Miniaudio::Miniaudio PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${MINIAUDIO_INCLUDE_DIR}
    IMPORTED_LOCATION ${MINIAUDIO_LIBRARY}
    COMPILE_OPTIONS "${PC_MINIAUDIO_CFLAGS_OTHER}"
 )
