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
SET(SYSTEM_LIB "sfml-system")

if(SFML_BUILD_WINDOW)
    SET(WINDOW_SRC
        "${SRCROOT}/TestUtilities/Window.hpp"
        "${SRCROOT}/TestUtilities/Window.cpp"
    )
    SET(WINDOW_LIB "sfml-window")
endif()

if(SFML_BUILD_GRAPHICS)
    SET(GRAPHICS_SRC
        "${SRCROOT}/Rect.cpp"
        "${SRCROOT}/TestUtilities/Graphics.hpp"
        "${SRCROOT}/TestUtilities/Graphics.cpp"
    )
    SET(WINDOW_LIB "sfml-graphics")
endif()

SET(SRC
    "${SRCROOT}/Main.cpp"
    "${SYSTEM_SRC}"
    "${WINDOW_SRC}"
    "${GRAPHICS_SRC}"
    "${AUDIO_SRC}"
    "${NETWORK_SRC}"
)

# Using sfmltest instead of test because the latter is reserved.
add_executable(sfmltest ${SRC})

target_link_libraries(sfmltest ${GRAPHICS_LIB} ${WINDOW_LIB} ${AUDIO_LIB} ${NETWORK_LIB} ${SYSTEM_LIB})

add_custom_target(runtests ALL
    DEPENDS sfmltest
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/test"
    COMMAND ${CMAKE_COMMAND} -E echo "Running test suite..."
    COMMAND sfmltest
)
