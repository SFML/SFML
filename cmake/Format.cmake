cmake_minimum_required(VERSION 3.28)
set(REQUIRED_VERSION 17.0.6)

# Get a binary for the required clang-format version
function(get_required_version)
    message(STATUS "Fetching clang-format ${REQUIRED_VERSION}")

    if(CMAKE_HOST_WIN32)
        set(ARTIFACT_NAME clang-format-windows-x64.exe)
        set(SHA256_HASH 502b914e65d47682f9641d083405092b4397f3768175470014072595a55c1699)
    elseif(CMAKE_HOST_APPLE)
        set(ARTIFACT_NAME clang-format-macos-arm64)
        set(SHA256_HASH b55b6f4199d0246166570612c3b51e6c7f880c957c6c72e602203fcc71e20d60)
    else()
        set(ARTIFACT_NAME clang-format-linux-arm64)
        set(SHA256_HASH 905589de6ae6d4e513363114679dfd87a0c5a57d3ef01ebe2c77c3fa13204cce)
    endif()

    set(CLANG_FORMAT_EXECUTABLE build/${ARTIFACT_NAME})
    file(DOWNLOAD https://github.com/eXpl0it3r/clang-format/releases/download/llvmorg-${REQUIRED_VERSION}/${ARTIFACT_NAME} ${CLANG_FORMAT_EXECUTABLE} 
        EXPECTED_HASH SHA256=${SHA256_HASH}
        SHOW_PROGRESS)
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

execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${SOURCES})
