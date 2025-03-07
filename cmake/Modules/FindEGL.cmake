#
# Try to find EGL library and include path.
# Once done this will define
#
# EGL_FOUND
# EGL_INCLUDE_PATH
# EGL_LIBRARY
#

find_path(EGL_INCLUDE_DIR EGL/egl.h)
find_library(EGL_LIBRARY NAMES EGL)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EGL DEFAULT_MSG EGL_LIBRARY EGL_INCLUDE_DIR)

add_library(EGL::EGL IMPORTED UNKNOWN)
set_target_properties(EGL::EGL PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${EGL_INCLUDE_DIR}
    IMPORTED_LOCATION ${EGL_LIBRARY})
