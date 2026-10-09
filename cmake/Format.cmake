cmake_minimum_required(VERSION 3.28)
set(REQUIRED_VERSION 17.0.6)

# Get a binary for the required clang-format version
function(get_required_version)
    message(STATUS "Fetching clang-format ${REQUIRED_VERSION}")

    # CMAKE_HOST_SYSTEM_PROCESSOR isn't set in script mode
    cmake_host_system_information(RESULT HOST_ARCH QUERY OS_PLATFORM)
    string(TOLOWER "${HOST_ARCH}" HOST_ARCH)

    if(CMAKE_HOST_WIN32)
        # Windows reports the architecture of the CMake process, so check the native one first
        if(DEFINED ENV{PROCESSOR_ARCHITEW6432})
            string(TOLOWER "$ENV{PROCESSOR_ARCHITEW6432}" HOST_ARCH)
        endif()

        if(HOST_ARCH STREQUAL "arm64")
            set(ARTIFACT_NAME clang-format-windows-arm64.exe)
            set(SHA256_HASH 2883f1d1ef4f4d164cff85dc8484cd5e25b54be26c327fff9d133c82946db797)
        elseif(HOST_ARCH STREQUAL "x86")
            set(ARTIFACT_NAME clang-format-windows-x86.exe)
            set(SHA256_HASH 1caa13cb90ae6beb8a1965dec49ba7d61090fa2b7e0bc4134b439343aebdd90a)
        else()
            set(ARTIFACT_NAME clang-format-windows-x64.exe)
            set(SHA256_HASH 502b914e65d47682f9641d083405092b4397f3768175470014072595a55c1699)
        endif()
    elseif(CMAKE_HOST_APPLE)
        # LLVM doesn't provide an Intel macOS build for 17.x
        if(NOT HOST_ARCH STREQUAL "arm64")
            message(FATAL_ERROR "No clang-format ${REQUIRED_VERSION} binary available for macOS ${HOST_ARCH}, install it manually")
        endif()

        set(ARTIFACT_NAME clang-format-macos-arm64)
        set(SHA256_HASH b55b6f4199d0246166570612c3b51e6c7f880c957c6c72e602203fcc71e20d60)
    elseif(HOST_ARCH MATCHES "^(aarch64|arm64)$")
        set(ARTIFACT_NAME clang-format-linux-arm64)
        set(SHA256_HASH 905589de6ae6d4e513363114679dfd87a0c5a57d3ef01ebe2c77c3fa13204cce)
    else()
        set(ARTIFACT_NAME clang-format-linux-x64)
        set(SHA256_HASH 9f0764b98ddfff5fbd7a6537dc8e6e6fc9cac211d8b021ad6c649db26ce0723a)
    endif()

    set(CLANG_FORMAT_EXECUTABLE build/${ARTIFACT_NAME})
    file(DOWNLOAD https://github.com/SFML/clang-tools/releases/download/llvmorg-${REQUIRED_VERSION}/${ARTIFACT_NAME} ${CLANG_FORMAT_EXECUTABLE}
        EXPECTED_HASH SHA256=${SHA256_HASH}
        SHOW_PROGRESS)

    # Downloaded files aren't executable by default
    file(CHMOD ${CLANG_FORMAT_EXECUTABLE} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    return(PROPAGATE CLANG_FORMAT_EXECUTABLE)
endfunction()

# First try finding clang-format on the system and check it's the required version
find_program(CLANG_FORMAT_EXECUTABLE clang-format)
if(CLANG_FORMAT_EXECUTABLE)
    execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} --version OUTPUT_VARIABLE CLANG_FORMAT_VERSION)
    string(REGEX MATCH "clang-format version ([0-9]+\.[0-9]+\.[0-9]+)" CLANG_FORMAT_VERSION ${CLANG_FORMAT_VERSION})
    unset(CLANG_FORMAT_VERSION)
    if(NOT CMAKE_MATCH_1 STREQUAL "${REQUIRED_VERSION}")
        message(WARNING "clang-format version ${CMAKE_MATCH_1} not supported. Must use version ${REQUIRED_VERSION}")
        get_required_version()
    endif()
else()
    message(WARNING "clang-format not found on system")
    get_required_version()
endif()

# Run
set(SOURCES "")
foreach(FOLDER IN ITEMS examples include src test tools)
    file(GLOB_RECURSE folder_files "${FOLDER}/*.h" "${FOLDER}/*.hpp" "${FOLDER}/*.inl" "${FOLDER}/*.cpp" "${FOLDER}/*.mm" "${FOLDER}/*.m")
    list(FILTER folder_files EXCLUDE REGEX "gl.h|vulkan.h|stb_perlin.h") # 3rd party code to exclude from formatting
    list(APPEND SOURCES ${folder_files})
endforeach()

# Format in batches, all sources at once exceed the command line length limit on Windows
list(LENGTH SOURCES SOURCE_COUNT)
set(BATCH_SIZE 100)
foreach(BATCH_START RANGE 0 ${SOURCE_COUNT} ${BATCH_SIZE})
    list(SUBLIST SOURCES ${BATCH_START} ${BATCH_SIZE} BATCH)
    if(BATCH)
        execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${BATCH} COMMAND_ERROR_IS_FATAL ANY)
    endif()
endforeach()
