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
sfml_add_test(test-sfml-system "${SYSTEM_SRC}"  sfml-system)

if(SFML_BUILD_WINDOW)
    SET(WINDOW_SRC
        "${SRCROOT}/CatchMain.cpp"
        "${SRCROOT}/TestUtilities/WindowUtil.hpp"
        "${SRCROOT}/TestUtilities/WindowUtil.cpp"
    )
    sfml_add_test(test-sfml-window "${WINDOW_SRC}" sfml-window)
endif()

if(SFML_BUILD_GRAPHICS)
    SET(GRAPHICS_SRC
        "${SRCROOT}/CatchMain.cpp"
        "${SRCROOT}/Graphics/Rect.cpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.hpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.cpp"
    )
    sfml_add_test(test-sfml-graphics "${GRAPHICS_SRC}" sfml-graphics)
endif()

if(SFML_BUILD_NETWORK)
    SET(NETWORK_SRC
        "${SRCROOT}/CatchMain.cpp"
        "${SRCROOT}/Network/Packet.cpp"
    )
    sfml_add_test(test-sfml-network "${NETWORK_SRC}" sfml-network)
endif()

# Automatically run the tests at the end of the build
add_custom_target(runtests ALL
                  DEPENDS test-sfml-system test-sfml-window test-sfml-graphics test-sfml-network
)

add_custom_command(TARGET runtests
                   COMMENT "Run tests"
                   POST_BUILD COMMAND ctest ARGS --output-on-failure
)
