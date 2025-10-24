set(MESA3D_URL "https://github.com/pal1000/mesa-dist-win/releases/download/25.2.0/mesa3d-25.2.0-release-msvc.7z")
set(MESA3D_SHA256 "67e76e9844206c71cf313e09409303af9c01d7561c5f57d7e152771e2408aafe")

cmake_path(GET MESA3D_URL FILENAME MESA3D_ARCHIVE)
cmake_path(GET MESA3D_URL STEM LAST_ONLY MESA3D_ARCHIVE_DIRECTORY)

if(ARCH_X64)
    set(MESA3D_ARCH "x64")
elseif(ARCH_X86)
    set(MESA3D_ARCH "x86")
elseif(SFML_USE_MESA3D)
    message(FATAL_ERROR "Mesa 3D does currently not support the target architecture.")
endif()

set(MESA3D_ARCHIVE_PATH "${PROJECT_BINARY_DIR}/${MESA3D_ARCHIVE_DIRECTORY}/${MESA3D_ARCHIVE}")
set(MESA3D_ARCH_PATH "${PROJECT_BINARY_DIR}/${MESA3D_ARCHIVE_DIRECTORY}/${MESA3D_ARCH}")

# we support automatically installing and uninstalling the necessary files

# if SFML_USE_MESA3D is set and true during configuration, we add custom commands to
# automatically copy over the necessary files whenever an executable/test target is built

# if SFML_USE_MESA3D is not set or false but the Mesa 3D directory is present, use its file list to
# remove any files that were previously copied into ${PROJECT_BINARY_DIR}/bin and subdirectories

if(SFML_OS_WINDOWS AND SFML_USE_MESA3D)
    # we are installing the files

    # if the Mesa 3D directory is not yet present, download and extract the
    # files relevant for the architecture we are configured to build for
    if(NOT EXISTS "${MESA3D_ARCH_PATH}")
        message(STATUS "Downloading ${MESA3D_ARCHIVE}")

        file(DOWNLOAD "${MESA3D_URL}" "${MESA3D_ARCHIVE_PATH}" SHOW_PROGRESS EXPECTED_HASH SHA256=${MESA3D_SHA256})

        if(NOT EXISTS "${MESA3D_ARCHIVE_PATH}")
            message(FATAL_ERROR "Failed to download ${MESA3D_URL}")
        endif()

        message(STATUS "Extracting ${MESA3D_ARCH} files from ${MESA3D_ARCHIVE}")

        execute_process(COMMAND "${CMAKE_COMMAND}" -E tar x "${MESA3D_ARCHIVE_PATH}" -- ${MESA3D_ARCH} WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/${MESA3D_ARCHIVE_DIRECTORY}")

        file(REMOVE "${MESA3D_ARCHIVE_PATH}")
    endif()

    # Create a list of dll's that can be copied where needed
    file(GLOB MESA3D_FILE_LIST "${MESA3D_ARCH_PATH}/*.dll")
endif()
