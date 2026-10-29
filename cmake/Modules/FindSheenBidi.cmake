#
# Try to find SheenBidi libraries and include paths.
# Once done this will define
#
# SHEENBIDI_FOUND
# SHEENBIDI_INCLUDE_DIR
# SHEENBIDI_LIBRARY
#

find_path(SHEENBIDI_INCLUDE_DIR SheenBidi/SheenBidi.h)
find_library(SHEENBIDI_LIBRARY NAMES SheenBidi)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SheenBidi DEFAULT_MSG SHEENBIDI_LIBRARY SHEENBIDI_INCLUDE_DIR)

mark_as_advanced(SHEENBIDI_INCLUDE_DIR SHEENBIDI_LIBRARY)

add_library(SheenBidi::SheenBidi IMPORTED UNKNOWN)
set_target_properties(SheenBidi::SheenBidi PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SHEENBIDI_INCLUDE_DIR})
if(SHEENBIDI_LIBRARY MATCHES "/([^/]+)\\.framework$")
    set_target_properties(SheenBidi::SheenBidi PROPERTIES IMPORTED_LOCATION ${SHEENBIDI_LIBRARY}/${CMAKE_MATCH_1})
else()
    set_target_properties(SheenBidi::SheenBidi PROPERTIES IMPORTED_LOCATION ${SHEENBIDI_LIBRARY})
endif()
