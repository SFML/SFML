add_library(sfml-test-main STATIC
    DoctestMain.cpp
    TestUtilities/SystemUtil.cpp
    TestUtilities/WindowUtil.cpp
    TestUtilities/GraphicsUtil.cpp
)
target_include_directories(sfml-test-main SYSTEM PUBLIC "${PROJECT_SOURCE_DIR}/extlibs/headers")
target_include_directories(sfml-test-main PUBLIC TestUtilities)
target_link_libraries(sfml-test-main PUBLIC SFML::System)

# System is always built
sfml_add_test(test-sfml-system SFML::System)
target_sources(test-sfml-system PRIVATE
    System/Angle.cpp
    System/Clock.cpp
    System/FileInputStream.cpp
    System/Time.cpp
    System/Vector2.cpp
    System/Vector3.cpp
)

if(SFML_BUILD_WINDOW)
    sfml_add_test(test-sfml-window SFML::Window)
    target_sources(test-sfml-window PRIVATE
        Window/ContextSettings.cpp
        Window/VideoMode.cpp
    )
endif()

if(SFML_BUILD_GRAPHICS)
    sfml_add_test(test-sfml-graphics SFML::Graphics)
    target_sources(test-sfml-graphics PRIVATE
        Graphics/BlendMode.cpp
        Graphics/Color.cpp
        Graphics/Rect.cpp
        Graphics/Transform.cpp
        Graphics/Transformable.cpp
        Graphics/Vertex.cpp
        Graphics/VertexArray.cpp
    )
endif()

if(SFML_BUILD_NETWORK)
    sfml_add_test(test-sfml-network SFML::Network)
    target_sources(test-sfml-network PRIVATE
        Network/Packet.cpp
    )
endif()

if(SFML_BUILD_AUDIO)
    sfml_add_test(test-sfml-audio SFML::Audio)
    target_sources(test-sfml-audio PRIVATE
        Audio/Dummy.cpp # TODO: Remove when there are real tests
    )
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
