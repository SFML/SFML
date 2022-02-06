set(SRCROOT "${PROJECT_SOURCE_DIR}/test")

add_library(sfml-test-main STATIC "${SRCROOT}/DoctestMain.cpp")
target_include_directories(sfml-test-main PUBLIC "${PROJECT_SOURCE_DIR}/extlibs/headers" "${SRCROOT}/TestUtilities")
target_compile_features(sfml-test-main PRIVATE cxx_std_17)

# System is always built
SET(SYSTEM_SRC
    "${SRCROOT}/System/Angle.cpp"
    "${SRCROOT}/System/Clock.cpp"
    "${SRCROOT}/System/FileInputStream.cpp"
    "${SRCROOT}/System/Time.cpp"
    "${SRCROOT}/System/Vector2.cpp"
    "${SRCROOT}/System/Vector3.cpp"
    "${SRCROOT}/TestUtilities/SystemUtil.hpp"
    "${SRCROOT}/TestUtilities/SystemUtil.cpp"
)
sfml_add_test(test-sfml-system "${SYSTEM_SRC}" SFML::System)
target_link_libraries(test-sfml-system PRIVATE sfml-test-main)

if(SFML_BUILD_WINDOW)
    SET(WINDOW_SRC
        "${SRCROOT}/Window/ContextSettings.cpp"
        "${SRCROOT}/Window/VideoMode.cpp"
        "${SRCROOT}/TestUtilities/WindowUtil.hpp"
        "${SRCROOT}/TestUtilities/WindowUtil.cpp"
    )
    sfml_add_test(test-sfml-window "${WINDOW_SRC}" SFML::Window)
    target_link_libraries(test-sfml-window PRIVATE sfml-test-main)
endif()

if(SFML_BUILD_GRAPHICS)
    SET(GRAPHICS_SRC
        "${SRCROOT}/Graphics/BlendMode.cpp"
        "${SRCROOT}/Graphics/Color.cpp"
        "${SRCROOT}/Graphics/Rect.cpp"
        "${SRCROOT}/Graphics/Transform.cpp"
        "${SRCROOT}/Graphics/Vertex.cpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.hpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.cpp"
    )
    sfml_add_test(test-sfml-graphics "${GRAPHICS_SRC}" SFML::Graphics)
    target_link_libraries(test-sfml-graphics PRIVATE sfml-test-main)
endif()

if(SFML_BUILD_NETWORK)
    SET(NETWORK_SRC
        "${SRCROOT}/Network/Packet.cpp"
    )
    sfml_add_test(test-sfml-network "${NETWORK_SRC}" SFML::Network)
    target_link_libraries(test-sfml-network PRIVATE sfml-test-main)
endif()

if(SFML_BUILD_AUDIO)
    SET(AUDIO_SRC
        "${SRCROOT}/Audio/Dummy.cpp" # TODO: Remove when there are real tests
    )
    sfml_add_test(test-sfml-audio "${AUDIO_SRC}" SFML::Audio)
    target_link_libraries(test-sfml-audio PRIVATE sfml-test-main)
endif()

# Automatically run the tests at the end of the build
add_custom_target(runtests ALL
                  DEPENDS test-sfml-system test-sfml-window test-sfml-graphics test-sfml-network test-sfml-audio
)

if(SFML_OS_WINDOWS AND NOT SFML_USE_SYSTEM_DEPS)
    # Copy the binaries of SFML dependencies
    list(APPEND BINARIES
        "openal32.dll"
    )

    foreach (BINARY ${BINARIES})
        if(ARCH_32BITS)
            list(APPEND BINARY_PATHS "${PROJECT_SOURCE_DIR}/extlibs/bin/x86/${BINARY}")
        elseif(ARCH_64BITS)
            list(APPEND BINARY_PATHS "${PROJECT_SOURCE_DIR}/extlibs/bin/x64/${BINARY}")
        endif()
    endforeach()

    add_custom_command(TARGET runtests
                       COMMENT "Copy binaries"
                       POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${BINARY_PATHS} "$<TARGET_FILE_DIR:test-sfml-system>"
    )
endif()

if(SFML_ENABLE_COVERAGE AND SFML_COMPILER_MSVC)
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

    add_custom_command(TARGET runtests
                       COMMENT "Run tests"
                       POST_BUILD COMMAND "${OpenCppCoverage_BINARY}" ARGS --quiet --export_type cobertura:"${CMAKE_BINARY_DIR}/coverage.out" --cover_children --excluded_modules "${COVERAGE_EXCLUDE}" --sources "${COVERAGE_SRC}" --sources "${COVERAGE_INCLUDE}" -- "${CMAKE_CTEST_COMMAND}" --output-on-failure -C $<CONFIG>
    )
else()
    # Run tests without a coverage runner
    add_custom_command(TARGET runtests
                       COMMENT "Run tests"
                       POST_BUILD COMMAND "${CMAKE_CTEST_COMMAND}" --output-on-failure -C $<CONFIG>
    )
endif()
