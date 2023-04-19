include(FetchContent)

set(DOCTEST_NO_INSTALL ON)
FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.9
)
FetchContent_MakeAvailable(doctest)
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

# Ensure that doctest sources and headers are not analyzed by any tools
set_target_properties(doctest_with_main PROPERTIES COMPILE_OPTIONS "" EXPORT_COMPILE_COMMANDS OFF)
get_target_property(DOCTEST_INCLUDE_DIRS doctest INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(doctest SYSTEM INTERFACE ${DOCTEST_INCLUDE_DIRS})

add_subdirectory(install)
set_target_warnings(test-sfml-install)

add_library(sfml-test-main STATIC
    TestUtilities/SystemUtil.hpp
    TestUtilities/SystemUtil.cpp
    TestUtilities/WindowUtil.hpp
    TestUtilities/WindowUtil.cpp
    TestUtilities/GraphicsUtil.hpp
    TestUtilities/GraphicsUtil.cpp
)
target_include_directories(sfml-test-main PUBLIC TestUtilities)
target_compile_definitions(sfml-test-main PUBLIC DOCTEST_CONFIG_REQUIRE_STRINGIFICATION_FOR_ALL_USED_TYPES)
target_link_libraries(sfml-test-main PUBLIC SFML::System doctest::doctest_with_main)
set_target_warnings(sfml-test-main)

set(SYSTEM_SRC
    System/Angle.test.cpp
    System/Clock.test.cpp
    System/Config.test.cpp
    System/Err.test.cpp
    System/FileInputStream.test.cpp
    System/MemoryInputStream.test.cpp
    System/Sleep.test.cpp
    System/String.test.cpp
    System/Time.test.cpp
    System/Vector2.test.cpp
    System/Vector3.test.cpp
)
sfml_add_test(test-sfml-system "${SYSTEM_SRC}" SFML::System)
target_compile_definitions(test-sfml-system PRIVATE
    EXPECTED_SFML_VERSION_MAJOR=${SFML_VERSION_MAJOR}
    EXPECTED_SFML_VERSION_MINOR=${SFML_VERSION_MINOR}
    EXPECTED_SFML_VERSION_PATCH=${SFML_VERSION_PATCH}
    EXPECTED_SFML_VERSION_IS_RELEASE=$<IF:$<BOOL:${VERSION_IS_RELEASE}>,true,false>
)

sfml_set_option(SFML_RUN_DISPLAY_TESTS ON BOOL "TRUE to run tests that require a display, FALSE to ignore it")

set(WINDOW_SRC
    Window/Context.test.cpp
    Window/ContextSettings.test.cpp
    Window/Cursor.test.cpp
    Window/GlResource.test.cpp
    Window/VideoMode.test.cpp
    Window/Window.test.cpp
    Window/WindowBase.test.cpp
)
sfml_add_test(test-sfml-window "${WINDOW_SRC}" SFML::Window)
if(SFML_RUN_DISPLAY_TESTS)
    target_compile_definitions(test-sfml-window PRIVATE SFML_RUN_DISPLAY_TESTS)
endif()

set(GRAPHICS_SRC
    Graphics/BlendMode.test.cpp
    Graphics/CircleShape.test.cpp
    Graphics/Color.test.cpp
    Graphics/ConvexShape.test.cpp
    Graphics/Drawable.test.cpp
    Graphics/Font.test.cpp
    Graphics/Glyph.test.cpp
    Graphics/Image.test.cpp
    Graphics/Rect.test.cpp
    Graphics/RectangleShape.test.cpp
    Graphics/RenderStates.test.cpp
    Graphics/RenderTarget.test.cpp
    Graphics/RenderTexture.test.cpp
    Graphics/RenderWindow.test.cpp
    Graphics/Shader.test.cpp
    Graphics/Shape.test.cpp
    Graphics/Sprite.test.cpp
    Graphics/Text.test.cpp
    Graphics/Texture.test.cpp
    Graphics/Transform.test.cpp
    Graphics/Transformable.test.cpp
    Graphics/Vertex.test.cpp
    Graphics/VertexArray.test.cpp
    Graphics/VertexBuffer.test.cpp
    Graphics/View.test.cpp
)
sfml_add_test(test-sfml-graphics "${GRAPHICS_SRC}" SFML::Graphics)
if(SFML_RUN_DISPLAY_TESTS)
    target_compile_definitions(test-sfml-graphics PRIVATE SFML_RUN_DISPLAY_TESTS)
endif()

set(NETWORK_SRC
    Network/Ftp.test.cpp
    Network/Http.test.cpp
    Network/IpAddress.test.cpp
    Network/Packet.test.cpp
    Network/Socket.test.cpp
    Network/SocketSelector.test.cpp
    Network/TcpListener.test.cpp
    Network/TcpSocket.test.cpp
    Network/UdpSocket.test.cpp
)
sfml_add_test(test-sfml-network "${NETWORK_SRC}" SFML::Network)

set(AUDIO_SRC
    Audio/AlResource.test.cpp
    Audio/InputSoundFile.test.cpp
    Audio/Music.test.cpp
    Audio/OutputSoundFile.test.cpp
    Audio/Sound.test.cpp
    Audio/SoundBuffer.test.cpp
    Audio/SoundBufferRecorder.test.cpp
    Audio/SoundRecorder.test.cpp
    Audio/SoundSource.test.cpp
    Audio/SoundStream.test.cpp
)
sfml_add_test(test-sfml-audio "${AUDIO_SRC}" SFML::Audio)

if(SFML_OS_WINDOWS AND NOT SFML_USE_SYSTEM_DEPS)
    add_custom_command(
        TARGET test-sfml-audio
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/extlibs/bin/$<IF:$<BOOL:${ARCH_64BITS}>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:test-sfml-audio>
        VERBATIM)
endif()

if(SFML_ENABLE_COVERAGE AND SFML_OS_WINDOWS)
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

    set(COVERAGE_PREFIX ${OpenCppCoverage_BINARY} ARGS --quiet --export_type cobertura:${PROJECT_BINARY_DIR}/coverage.out --cover_children --excluded_modules "${COVERAGE_EXCLUDE}" --sources "${COVERAGE_SRC}" --sources "${COVERAGE_INCLUDE}" --)
endif()

# Convenience for building and running tests in a single command
add_custom_target(runtests DEPENDS test-sfml-system test-sfml-window test-sfml-graphics test-sfml-network test-sfml-audio)
add_custom_command(TARGET runtests
                   COMMENT "Run tests"
                   POST_BUILD COMMAND ${COVERAGE_PREFIX} ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>
                   VERBATIM)
