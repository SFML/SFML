include(FetchContent)

set(CATCH_CONFIG_NO_EXPERIMENTAL_STATIC_ANALYSIS_SUPPORT ON CACHE BOOL "")
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.9.1
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(Catch2)
include(Catch)

# Build Catch2 in C++17 mode to enable C++17 features
target_compile_features(Catch2 PRIVATE cxx_std_17)

# Ensure that Catch2 sources and headers are not analyzed by any tools
set_target_properties(Catch2 PROPERTIES COMPILE_OPTIONS "" EXPORT_COMPILE_COMMANDS OFF UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE 256)
set_target_properties(Catch2WithMain PROPERTIES EXPORT_COMPILE_COMMANDS OFF)
set_target_properties(Catch2 Catch2WithMain PROPERTIES FOLDER "Dependencies")
get_target_property(CATCH2_INCLUDE_DIRS Catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(Catch2 SYSTEM INTERFACE ${CATCH2_INCLUDE_DIRS})

add_library(sfml-test-main STATIC
    TestUtilities/SystemUtil.hpp
    TestUtilities/SystemUtil.cpp
    TestUtilities/NetworkUtil.hpp
    TestUtilities/NetworkUtil.cpp
    TestUtilities/WindowUtil.hpp
    TestUtilities/WindowUtil.cpp
    TestUtilities/GraphicsUtil.hpp
    TestUtilities/GraphicsUtil.cpp
    TestUtilities/AudioUtil.hpp
    TestUtilities/AudioUtil.cpp
)
target_include_directories(sfml-test-main PUBLIC TestUtilities)
target_link_libraries(sfml-test-main PUBLIC SFML::System)

# Main is required on some platforms and is completely removed on others, so we can only depend on it when it exists
if(TARGET SFML::Main)
    target_link_libraries(sfml-test-main PUBLIC SFML::Main Catch2::Catch2)
else()
    target_link_libraries(sfml-test-main PUBLIC Catch2::Catch2WithMain)
endif()

set_target_warnings(sfml-test-main)

# set the target flags to use the appropriate C++ standard library
sfml_set_stdlib(Catch2)
sfml_set_stdlib(Catch2WithMain)
sfml_set_stdlib(sfml-test-main)

sfml_set_option(SFML_RUN_DISPLAY_TESTS ON BOOL "ON to run tests that require a display, OFF to ignore it")
if(SFML_RUN_DISPLAY_TESTS)
    target_compile_definitions(sfml-test-main PRIVATE SFML_RUN_DISPLAY_TESTS)
endif()

sfml_set_option(SFML_RUN_AUDIO_DEVICE_TESTS ON BOOL "ON to run tests that require an audio device, OFF to ignore it")
if(SFML_RUN_AUDIO_DEVICE_TESTS)
    target_compile_definitions(sfml-test-main PRIVATE SFML_RUN_AUDIO_DEVICE_TESTS)
endif()

if(NOT SFML_OS_IOS)
    target_compile_definitions(sfml-test-main PRIVATE SFML_RUN_CONNECTION_TESTS)
    target_compile_definitions(sfml-test-main PRIVATE SFML_RUN_LOOPBACK_TESTS)
endif()

add_subdirectory(Audio)
add_subdirectory(Graphics)
add_subdirectory(Network)
add_subdirectory(System)
add_subdirectory(Window)

if(SFML_OS_ANDROID)
    # We can only write to the tmp directory on the Android virtual device
    set(TARGET_DIR "/data/local/tmp")

    # adb is required to push to device when running tests on the command line
    # which not everybody will want/need to do, so only cause an error when trying to do that
    find_program(ADB adb)

    if(ADB)
        # Copy binaries for all the required targets
        add_custom_target(prepare-android-files
            COMMAND ${ADB} push "$<TARGET_FILE:test-sfml-audio>"        ${TARGET_DIR}/$<TARGET_FILE_NAME:test-sfml-audio>
            COMMAND ${ADB} push "$<TARGET_FILE:test-sfml-graphics>"     ${TARGET_DIR}/$<TARGET_FILE_NAME:test-sfml-graphics>
            COMMAND ${ADB} push "$<TARGET_FILE:test-sfml-network>"      ${TARGET_DIR}/$<TARGET_FILE_NAME:test-sfml-network>
            COMMAND ${ADB} push "$<TARGET_FILE:test-sfml-system>"       ${TARGET_DIR}/$<TARGET_FILE_NAME:test-sfml-system>
            COMMAND ${ADB} push "$<TARGET_FILE:test-sfml-window>"       ${TARGET_DIR}/$<TARGET_FILE_NAME:test-sfml-window>
            COMMAND ${ADB} push "$<TARGET_FILE:SFML::Audio>"            ${TARGET_DIR}/$<TARGET_FILE_NAME:SFML::Audio>
            COMMAND ${ADB} push "$<TARGET_FILE:SFML::Graphics>"         ${TARGET_DIR}/$<TARGET_FILE_NAME:SFML::Graphics>
            COMMAND ${ADB} push "$<TARGET_FILE:SFML::Network>"          ${TARGET_DIR}/$<TARGET_FILE_NAME:SFML::Network>
            COMMAND ${ADB} push "$<TARGET_FILE:SFML::System>"           ${TARGET_DIR}/$<TARGET_FILE_NAME:SFML::System>
            COMMAND ${ADB} push "$<TARGET_FILE:SFML::Window>"           ${TARGET_DIR}/$<TARGET_FILE_NAME:SFML::Window>
            COMMAND ${ADB} push "$<TARGET_FILE:Catch2>"                 ${TARGET_DIR}/$<TARGET_FILE_NAME:Catch2>
            COMMAND ${ADB} push "$<TARGET_FILE:Catch2WithMain>"         ${TARGET_DIR}/$<TARGET_FILE_NAME:Catch2WithMain>
            COMMAND ${ADB} push "${CMAKE_CURRENT_LIST_DIR}/Audio/."     ${TARGET_DIR}
            COMMAND ${ADB} push "${CMAKE_CURRENT_LIST_DIR}/Graphics/."  ${TARGET_DIR}
            COMMAND ${ADB} push "${CMAKE_CURRENT_LIST_DIR}/System/."    ${TARGET_DIR}
            COMMAND ${ADB} push "${CMAKE_CURRENT_LIST_DIR}/Audio/."     ${TARGET_DIR}
            COMMAND ${ADB} shell "chmod -R 775            ${TARGET_DIR}"
            COMMAND ${ADB} shell "ls -la                  ${TARGET_DIR}"
        )

        # Generate proxy script that translates CTest commands into adb shell commands
        if(CMAKE_HOST_WIN32)
            file(GENERATE OUTPUT "${PROJECT_BINARY_DIR}/run-in-adb-shell.bat" CONTENT
                "@echo off\n\
                for %%f in (\"%1\") do set \"exe=%%~nxf\"\n\
                adb shell \"cd ${TARGET_DIR}; LD_LIBRARY_PATH=${TARGET_DIR} /data/local/tmp/%exe% \\\"%2\\\" \\\"%3\\\" \\\"%4\\\" \\\"%5\\\" \\\"%6\\\" \\\"%7\\\" \\\"%8\\\" \\\"%9\\\"\"\n"
                FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
        else()
            file(GENERATE OUTPUT "${PROJECT_BINARY_DIR}/run-in-adb-shell.sh" CONTENT
                "#!/bin/bash\n\
                exe=$(basename $1)
                adb shell \"cd ${TARGET_DIR}; LD_LIBRARY_PATH=${TARGET_DIR} /data/local/tmp/$exe \\\"$2\\\" \\\"$3\\\" \\\"$4\\\" \\\"$5\\\" \\\"$6\\\" \\\"$7\\\" \\\"$8\\\" \\\"$9\\\"\"\n"
                FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
        endif()

        # If using shared STL that also needs to be copied
        if(CMAKE_ANDROID_STL_TYPE STREQUAL "c++_shared")
            # Find it in the ndk first. It's located in a path specific to both host and target arch
            if(CMAKE_HOST_WIN32)
                set(HOST_FOLDER "windows-x86_64")
            elseif(CMAKE_HOST_APPLE)
                set(HOST_FOLDER "darwin-x86_64")
            else()
                set(HOST_FOLDER "linux-x86_64")
            endif()

            if(CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
                set(TARGET_FOLDER "aarch64-linux-android")
            elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "armeabi-v7a")
                set(TARGET_FOLDER "arm-linux-androideabi")
            elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")
                set(TARGET_FOLDER "x86_64-linux-android")
            elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "x86")
                set(TARGET_FOLDER "i686-linux-android")
            endif()

            find_file(STL_LIB libc++_shared.so PATHS
                ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/${HOST_FOLDER}/sysroot/usr/lib/${TARGET_FOLDER}
                REQUIRED)
            add_custom_command(TARGET prepare-android-files POST_BUILD
                COMMAND ${ADB} push "${STL_LIB}" ${TARGET_DIR}/libc++_shared.so)
        endif()
    else()
        # Error message and fail
        add_custom_target(prepare-android-files
            COMMAND ${CMAKE_COMMAND} -E echo "adb wasn't found, install it and reconfigure"
            COMMAND ${CMAKE_COMMAND} -E false)
    endif()
endif()

if(SFML_ENABLE_COVERAGE AND SFML_OS_WINDOWS AND NOT SFML_COMPILER_GCC)
    # Try to find and use OpenCppCoverage for coverage reporting when building with MSVC
    find_program(OpenCppCoverage_BINARY "OpenCppCoverage.exe")

    if(OpenCppCoverage_BINARY)
        execute_process(COMMAND "${OpenCppCoverage_BINARY}" --help ERROR_VARIABLE OpenCppCoverage_HELP_OUTPUT OUTPUT_QUIET)

        if(OpenCppCoverage_HELP_OUTPUT MATCHES "OpenCppCoverage Version: ([.0-9]+)")
            set(OpenCppCoverage_VERSION "${CMAKE_MATCH_1}")
        endif()
    endif()

    include(FindPackageHandleStandardArgs)

    find_package_handle_standard_args(OpenCppCoverage
        REQUIRED_VARS OpenCppCoverage_BINARY
        VERSION_VAR OpenCppCoverage_VERSION
    )
endif()

if(SFML_ENABLE_COVERAGE AND OpenCppCoverage_FOUND)
    # Use OpenCppCoverage
    message(STATUS "Using OpenCppCoverage to generate coverage report")

    string(REPLACE "/" "\\" COVERAGE_EXCLUDE "${CMAKE_CTEST_COMMAND}")
    string(REPLACE "/" "\\" COVERAGE_SRC "${PROJECT_SOURCE_DIR}/src")
    string(REPLACE "/" "\\" COVERAGE_INCLUDE "${PROJECT_SOURCE_DIR}/include")

    # We need to patch the OpenCppCoverage output to remove path prefixes so Coveralls doesn't get confused
    cmake_path(GET PROJECT_SOURCE_DIR ROOT_NAME COVERAGE_ROOT_NAME)
    string(REPLACE "/" "\\\\" COVERAGE_PATH_PREFIX "${PROJECT_SOURCE_DIR}/")
    string(REPLACE "${COVERAGE_ROOT_NAME}\\\\" "" COVERAGE_PATH_PREFIX "${COVERAGE_PATH_PREFIX}")

    file(WRITE "${PROJECT_BINARY_DIR}/patch_coverage.cmake"
         "file(READ \"${PROJECT_BINARY_DIR}/coverage.out\" COVERAGE_OUT)\n\
         string(REPLACE \"${COVERAGE_PATH_PREFIX}\" \"\" COVERAGE_OUT \"\${COVERAGE_OUT}\")\n\
         string(REPLACE \"${COVERAGE_ROOT_NAME}\" \".\" COVERAGE_OUT \"\${COVERAGE_OUT}\")\n\
         file(WRITE \"${PROJECT_BINARY_DIR}/coverage.out\" \"\${COVERAGE_OUT}\")\n")

    set(COVERAGE_PREFIX ${OpenCppCoverage_BINARY} --quiet --export_type cobertura:${PROJECT_BINARY_DIR}/coverage.out --cover_children --excluded_modules "${COVERAGE_EXCLUDE}" --sources "${COVERAGE_SRC}" --sources "${COVERAGE_INCLUDE}" --)
else()
    # On all other systems, we just run an empty script
    file(WRITE "${PROJECT_BINARY_DIR}/patch_coverage.cmake" "")
endif()

# Convenience for building and running tests in a single command
add_custom_target(runtests DEPENDS test-sfml-system test-sfml-window test-sfml-graphics test-sfml-network test-sfml-audio)
add_custom_command(TARGET runtests
                   COMMENT "Run tests"
                   POST_BUILD
                   COMMAND ${COVERAGE_PREFIX} ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>
                   COMMAND ${CMAKE_COMMAND} -P "${PROJECT_BINARY_DIR}/patch_coverage.cmake"
                   VERBATIM)
