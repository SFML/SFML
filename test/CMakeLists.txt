set(SRCROOT "${PROJECT_SOURCE_DIR}/test/src")

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/extlibs/headers")
include_directories("${SRCROOT}/TestUtilities")

# System is always built
SET(SYSTEM_SRC
    "${SRCROOT}/CatchMain.cpp"
    "${SRCROOT}/System/Vector2.cpp"
    "${SRCROOT}/System/Vector3.cpp"
    "${SRCROOT}/TestUtilities/SystemUtil.hpp"
    "${SRCROOT}/TestUtilities/SystemUtil.cpp"
)
add_executable(systemtest ${SYSTEM_SRC})
target_link_libraries(systemtest sfml-system)
add_test(System-module-tests systemtest)

if(SFML_BUILD_WINDOW)
    SET(WINDOW_SRC
        "${SRCROOT}/CatchMain.cpp"
        "${SRCROOT}/TestUtilities/WindowUtil.hpp"
        "${SRCROOT}/TestUtilities/WindowUtil.cpp"
    )
    add_executable(windowtest ${WINDOW_SRC})
    target_link_libraries(windowtest sfml-window)
    add_test(Window-module-tests windowtest)
endif()

if(SFML_BUILD_GRAPHICS)
    SET(GRAPHICS_SRC
        "${SRCROOT}/CatchMain.cpp"
        "${SRCROOT}/Graphics/Rect.cpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.hpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.cpp"
    )
    add_executable(graphicstest ${GRAPHICS_SRC})
    target_link_libraries(graphicstest sfml-graphics)
    add_test(Graphics-module-tests graphicstest)
endif()

# Automatically run the tests at the end of the build
add_custom_target(runtests ALL
                  DEPENDS systemtest windowtest graphicstest
)
add_custom_command(TARGET runtests
                   COMMENT "Run tests"
                   POST_BUILD COMMAND ctest ARGS --output-on-failure
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
