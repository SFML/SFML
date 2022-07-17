include(FetchContent)

set(DOCTEST_NO_INSTALL ON)
FetchContent_Declare(doctest
    GIT_REPOSITORY "https://github.com/doctest/doctest.git"
    GIT_TAG v2.4.9
)
FetchContent_MakeAvailable(doctest)
list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)
include(doctest)

add_library(sfml-test-main STATIC
    DoctestMain.cpp
    TestUtilities/SystemUtil.hpp
    TestUtilities/SystemUtil.cpp
    TestUtilities/WindowUtil.hpp
    TestUtilities/WindowUtil.cpp
    TestUtilities/GraphicsUtil.hpp
    TestUtilities/GraphicsUtil.cpp
)
target_include_directories(sfml-test-main PUBLIC TestUtilities)
target_compile_definitions(sfml-test-main PUBLIC DOCTEST_CONFIG_REQUIRE_STRINGIFICATION_FOR_ALL_USED_TYPES)
target_link_libraries(sfml-test-main PUBLIC SFML::System doctest::doctest)
set_target_warnings(sfml-test-main)

set(SYSTEM_SRC
    System/Angle.cpp
    System/Clock.cpp
    System/Config.cpp
    System/Err.cpp
    System/FileInputStream.cpp
    System/MemoryInputStream.cpp
    System/Time.cpp
    System/Vector2.cpp
    System/Vector3.cpp
)
sfml_add_test(test-sfml-system "${SYSTEM_SRC}" SFML::System)
target_compile_definitions(test-sfml-system PRIVATE
    EXPECTED_SFML_VERSION_MAJOR=${SFML_VERSION_MAJOR}
    EXPECTED_SFML_VERSION_MINOR=${SFML_VERSION_MINOR}
    EXPECTED_SFML_VERSION_PATCH=${SFML_VERSION_PATCH}
    EXPECTED_SFML_VERSION_IS_RELEASE=$<IF:$<BOOL:${VERSION_IS_RELEASE}>,true,false>
)

set(WINDOW_SRC
    Window/ContextSettings.cpp
    Window/VideoMode.cpp
)
sfml_add_test(test-sfml-window "${WINDOW_SRC}" SFML::Window)

set(GRAPHICS_SRC
    Graphics/BlendMode.cpp
    Graphics/CircleShape.cpp
    Graphics/Color.cpp
    Graphics/ConvexShape.cpp
    Graphics/Glyph.cpp
    Graphics/Image.cpp
    Graphics/Rect.cpp
    Graphics/RectangleShape.cpp
    Graphics/Shape.cpp
    Graphics/RenderStates.cpp
    Graphics/Transform.cpp
    Graphics/Transformable.cpp
    Graphics/Vertex.cpp
    Graphics/VertexArray.cpp
    Graphics/View.cpp
)
sfml_add_test(test-sfml-graphics "${GRAPHICS_SRC}" SFML::Graphics)

set(NETWORK_SRC
    Network/IpAddress.cpp
    Network/Packet.cpp
)
sfml_add_test(test-sfml-network "${NETWORK_SRC}" SFML::Network)

set(AUDIO_SRC
    Audio/Dummy.cpp # TODO: Remove when there are real tests
)
sfml_add_test(test-sfml-audio "${AUDIO_SRC}" SFML::Audio)

if(SFML_OS_WINDOWS AND NOT SFML_USE_SYSTEM_DEPS)
    add_custom_command(
        TARGET test-sfml-audio
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/extlibs/bin/$<IF:$<BOOL:${ARCH_64BITS}>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:test-sfml-audio>
    )
endif()

# Automatically run the tests at the end of the build
add_custom_target(runtests ALL
                  DEPENDS test-sfml-system test-sfml-window test-sfml-graphics test-sfml-network test-sfml-audio
)

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
