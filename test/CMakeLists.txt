# Include the extra catch cmake functions
include("${CMAKE_SOURCE_DIR}/cmake/Catch.cmake")

set(SRCROOT "${PROJECT_SOURCE_DIR}/test/src")

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/extlibs/headers")

# System is always built
SET(SYSTEM_SRC
    "${SRCROOT}/Vector2.cpp"
    "${SRCROOT}/Vector3.cpp"
    "${SRCROOT}/TestUtilities/System.hpp"
    "${SRCROOT}/TestUtilities/System.cpp"
)
add_executable(systemtest ${SYSTEM_SRC})
target_link_libraries(systemtest sfml-system)
catch_discover_tests(systemtest)

if(SFML_BUILD_WINDOW)
    SET(WINDOW_SRC
        "${SRCROOT}/Window.cpp"
        "${SRCROOT}/TestUtilities/Window.hpp"
        "${SRCROOT}/TestUtilities/Window.cpp"
    )
    add_executable(windowtest ${WINDOW_SRC})
    target_link_libraries(windowtest sfml-window)
    catch_discover_tests(windowtest)
endif()

if(SFML_BUILD_GRAPHICS)
    SET(GRAPHICS_SRC
        "${SRCROOT}/Rect.cpp"
        "${SRCROOT}/TestUtilities/Graphics.hpp"
        "${SRCROOT}/TestUtilities/Graphics.cpp"
    )
    add_executable(graphicstest ${GRAPHICS_SRC})
    target_link_libraries(graphicstest sfml-graphics)
    catch_discover_tests(graphicstest)
endif()

# Automatically run the tests at the end of the build
add_custom_target( runtests ALL
                   DEPENDS systemtest windowtest graphicstest
)
add_custom_command(TARGET runtests
                   COMMENT "Run tests"
                   POST_BUILD COMMAND ctest ARGS --output-on-failure
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
