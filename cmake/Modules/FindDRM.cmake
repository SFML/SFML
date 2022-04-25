#
# Try to find EGL library and include path.
# Once done this will define
#
# DRM_FOUND
# DRM_INCLUDE_PATH
# DRM_LIBRARY
#

if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_DRM drm QUIET)
endif()

find_path(DRM_INCLUDE_DIR NAMES libdrm/drm.h)
find_library(DRM_LIBRARY NAMES drm)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DRM DEFAULT_MSG DRM_LIBRARY DRM_INCLUDE_DIR)
