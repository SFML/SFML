include(FetchContent)

set(DOCTEST_NO_INSTALL ON)
FetchContent_Declare(doctest
    GIT_REPOSITORY "https://github.com/doctest/doctest.git"
    GIT_TAG v2.4.9
)
FetchContent_MakeAvailable(doctest)

# add a new static library which provides a main function and testing utilities for SFML types
# example: sfml_add_test_utils(sfml-window-test-utils
#                              SOURCES TestUtilities/WindowUtil.hpp TestUtilities/WindowUtil.cpp
#                              DEPENDS SFML::Window sfml-system-test-utils)
function(sfml_add_test_utils target)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling sfml_add_test_utils: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    # set a source group for the source files
    source_group("" FILES ${THIS_SOURCES})

    # create the target
    add_library(${target} STATIC ${THIS_SOURCES})

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Tests/Utilities")

    # link the target to its dependencies
    target_link_libraries(${target} PUBLIC ${THIS_DEPENDS})

    set_target_warnings(${target})
endfunction()

# add a new target which is a test executable for an SFML module
# example: sfml_add_test(Network
#                        SOURCES ftp.cpp ...
#                        DEPENDS SFML::Network sfml-system-test-utils)
function(sfml_add_test module)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling sfml_add_test: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    # set a source group for the source files
    source_group("" FILES ${THIS_SOURCES})

    # create the target
    string(TOLOWER test-sfml-${module} target)
    add_executable(${target} ${THIS_SOURCES})

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Tests")

    # link the target to its dependencies
    target_link_libraries(${target} PRIVATE ${THIS_DEPENDS})

    set_target_warnings(${target})

    # If coverage is enabled for MSVC and we are linking statically, use /WHOLEARCHIVE
    # to make sure the linker doesn't discard unused code sections in the module under test
    # before coverage can be measured
    if (SFML_ENABLE_COVERAGE AND SFML_COMPILER_MSVC AND NOT BUILD_SHARED_LIBS)
        target_link_options(${target} PRIVATE $<$<CONFIG:DEBUG>:/WHOLEARCHIVE:$<TARGET_LINKER_FILE:SFML::${module}>>)
    endif()

    # Add the test
    add_test(${target} ${target})

    # Require this test to be built before automatically running tests
    add_dependencies(runtests ${target})

    # If building shared libs on windows we must copy the library under test into the folder
    if (WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET ${target} PRE_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy
                           $<TARGET_FILE:SFML::${module}>
                           $<TARGET_FILE_DIR:${target}>)
    endif()
endfunction()

# Target to automatically run the tests at the end of the build
# Its dependencies are added by sfml_add_test for enabled modules
add_custom_target(runtests ALL)

# System module is always built
sfml_add_test_utils(sfml-system-test-utils
    SOURCES DoctestMain.cpp
            TestUtilities/SystemUtil.hpp
            TestUtilities/SystemUtil.cpp
    DEPENDS SFML::System doctest::doctest
)
target_include_directories(sfml-system-test-utils PUBLIC TestUtilities)
target_compile_definitions(sfml-system-test-utils PUBLIC DOCTEST_CONFIG_REQUIRE_STRINGIFICATION_FOR_ALL_USED_TYPES)

sfml_add_test(System
    SOURCES System/Angle.cpp
            System/Clock.cpp
            System/Err.cpp
            System/FileInputStream.cpp
            System/MemoryInputStream.cpp
            System/Time.cpp
            System/Vector2.cpp
            System/Vector3.cpp
    DEPENDS sfml-system-test-utils
)
target_compile_definitions(test-sfml-system PRIVATE
    EXPECTED_SFML_VERSION_MAJOR=${SFML_VERSION_MAJOR}
    EXPECTED_SFML_VERSION_MINOR=${SFML_VERSION_MINOR}
    EXPECTED_SFML_VERSION_PATCH=${SFML_VERSION_PATCH}
    EXPECTED_SFML_VERSION_IS_RELEASE=$<IF:$<BOOL:${VERSION_IS_RELEASE}>,true,false>
)

if(SFML_BUILD_WINDOW)
    sfml_add_test_utils(sfml-window-test-utils
        SOURCES TestUtilities/WindowUtil.hpp
                TestUtilities/WindowUtil.cpp
        DEPENDS SFML::Window sfml-system-test-utils
    )

    sfml_add_test(Window
        SOURCES Window/ContextSettings.cpp
                Window/VideoMode.cpp
        DEPENDS sfml-window-test-utils
    )
endif()

if(SFML_BUILD_GRAPHICS)
    sfml_add_test_utils(sfml-graphics-test-utils
        SOURCES TestUtilities/GraphicsUtil.hpp
                TestUtilities/GraphicsUtil.cpp
        DEPENDS SFML::Graphics sfml-window-test-utils
    )

    sfml_add_test(Graphics
        SOURCES Graphics/BlendMode.cpp
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
        DEPENDS sfml-graphics-test-utils
    )
endif()

if(SFML_BUILD_NETWORK)
    sfml_add_test(Network
        SOURCES Network/IpAddress.cpp
                Network/Packet.cpp
        DEPENDS SFML::Network sfml-system-test-utils
    )
endif()

if(SFML_BUILD_AUDIO)
    sfml_add_test(Audio
        SOURCES Audio/Dummy.cpp # TODO: Remove when there are real tests
        DEPENDS SFML::Audio sfml-system-test-utils
    )
endif()

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
