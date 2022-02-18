add_library(sfml-test-main STATIC
    DoctestMain.cpp
    TestUtilities/SystemUtil.hpp
    TestUtilities/SystemUtil.cpp
    TestUtilities/WindowUtil.hpp
    TestUtilities/WindowUtil.cpp
    TestUtilities/GraphicsUtil.hpp
    TestUtilities/GraphicsUtil.cpp
)
target_include_directories(sfml-test-main PUBLIC "${PROJECT_SOURCE_DIR}/extlibs/headers" TestUtilities)
target_link_libraries(sfml-test-main PUBLIC SFML::System)

# System is always built
SET(SYSTEM_SRC
    System/Angle.cpp
    System/Clock.cpp
    System/FileInputStream.cpp
    System/Time.cpp
    System/Vector2.cpp
    System/Vector3.cpp
)
sfml_add_test(test-sfml-system "${SYSTEM_SRC}" SFML::System)

if(SFML_BUILD_WINDOW)
    SET(WINDOW_SRC
        Window/ContextSettings.cpp
        Window/VideoMode.cpp
    )
    sfml_add_test(test-sfml-window "${WINDOW_SRC}" SFML::Window)
endif()

if(SFML_BUILD_GRAPHICS)
    SET(GRAPHICS_SRC
        Graphics/BlendMode.cpp
        Graphics/Color.cpp
        Graphics/Rect.cpp
        Graphics/Transform.cpp
        Graphics/Transformable.cpp
        Graphics/Vertex.cpp
    )
    sfml_add_test(test-sfml-graphics "${GRAPHICS_SRC}" SFML::Graphics)
endif()

if(SFML_BUILD_NETWORK)
    SET(NETWORK_SRC
        Network/Packet.cpp
    )
    sfml_add_test(test-sfml-network "${NETWORK_SRC}" SFML::Network)
endif()

if(SFML_BUILD_AUDIO)
    SET(AUDIO_SRC
        Audio/Dummy.cpp # TODO: Remove when there are real tests
    )
    sfml_add_test(test-sfml-audio "${AUDIO_SRC}" SFML::Audio)
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
