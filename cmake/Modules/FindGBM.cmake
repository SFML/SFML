#
# Try to find GBM library and include path.
# Once done this will define
#
# GBM_FOUND
# GBM_INCLUDE_PATH
# GBM_LIBRARY
#

if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_GBM gbm QUIET)
endif()

find_path(GBM_INCLUDE_DIR NAMES gbm.h)
find_library(GBM_LIBRARY NAMES gbm)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GBM DEFAULT_MSG GBM_LIBRARY GBM_INCLUDE_DIR)
