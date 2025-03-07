set(MESA3D_URL "https://github.com/pal1000/mesa-dist-win/releases/download/23.0.0/mesa3d-23.0.0-release-msvc.7z")
set(MESA3D_SHA256 "FEF8A643689414A70347AE8027D24674DEFD85E8D6428C8A9D4145BB3F44A3B0")

get_filename_component(MESA3D_ARCHIVE "${MESA3D_URL}" NAME)
get_filename_component(MESA3D_ARCHIVE_DIRECTORY "${MESA3D_URL}" NAME_WLE)

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

    # add the files as file dependencies to a custom target that we can add as a dependency to executable/test targets
    file(GLOB MESA3D_FILE_LIST "${MESA3D_ARCH_PATH}/*")

    get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

    foreach(MESA3D_FILE ${MESA3D_FILE_LIST})
        get_filename_component(MESA3D_FILE_NAME "${MESA3D_FILE}" NAME)

        list(APPEND MESA3D_INSTALLED_FILES "${PROJECT_BINARY_DIR}/bin/$<IF:$<BOOL:${IS_MULTI_CONFIG}>,$<CONFIG>/,>${MESA3D_FILE_NAME}")
    endforeach()

    # if files are missing from the target directory of the configuration being built, copy them over
    add_custom_command(OUTPUT ${MESA3D_INSTALLED_FILES} COMMAND "${CMAKE_COMMAND}" ARGS -E copy_if_different ${MESA3D_FILE_LIST} "${PROJECT_BINARY_DIR}/bin$<IF:$<BOOL:${IS_MULTI_CONFIG}>,/$<CONFIG>,>")

    add_custom_target(install-mesa3d DEPENDS ${MESA3D_INSTALLED_FILES})

    set_target_properties(install-mesa3d PROPERTIES EXCLUDE_FROM_ALL ON)
elseif(SFML_OS_WINDOWS AND MESA3D_ARCH AND EXISTS "${MESA3D_ARCH_PATH}")
    # we are removing the files

    # compile a list of file names that we have to remove
    file(GLOB MESA3D_FILE_LIST "${MESA3D_ARCH_PATH}/*")

    foreach(MESA3D_FILE ${MESA3D_FILE_LIST})
        get_filename_component(MESA3D_FILE_NAME "${MESA3D_FILE}" NAME)

        list(APPEND MESA3D_FILE_NAMES "${MESA3D_FILE_NAME}")
    endforeach()

    # recursively go through all files in bin and remove files that match the file name of a Mesa 3D file
    file(GLOB_RECURSE BINARY_FILE_LIST "${PROJECT_BINARY_DIR}/bin/*")

    foreach(BINARY_FILE ${BINARY_FILE_LIST})
        get_filename_component(BINARY_FILE_NAME "${BINARY_FILE}" NAME)

        list(FIND MESA3D_FILE_NAMES "${BINARY_FILE_NAME}" INDEX)

        if(NOT INDEX EQUAL -1)
            file(REMOVE "${BINARY_FILE}")
        endif()
    endforeach()
endif()
