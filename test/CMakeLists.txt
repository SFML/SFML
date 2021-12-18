set(SRCROOT "${PROJECT_SOURCE_DIR}/test/src")

add_library(sfml-test-config "${SRCROOT}/CatchMain.cpp")
target_include_directories(sfml-test-config PUBLIC "${PROJECT_SOURCE_DIR}/extlibs/headers" "${SRCROOT}/TestUtilities")
target_compile_features(sfml-test-config PRIVATE cxx_std_17)

# System is always built
SET(SYSTEM_SRC
    "${SRCROOT}/System/Vector2.cpp"
    "${SRCROOT}/System/Vector3.cpp"
    "${SRCROOT}/TestUtilities/SystemUtil.hpp"
    "${SRCROOT}/TestUtilities/SystemUtil.cpp"
)

sfml_add_test(test-sfml-system "${SYSTEM_SRC}" sfml-system)
target_link_libraries(test-sfml-system PRIVATE sfml-test-config)

if(SFML_BUILD_WINDOW)
    SET(WINDOW_SRC
        "${SRCROOT}/TestUtilities/WindowUtil.hpp"
        "${SRCROOT}/TestUtilities/WindowUtil.cpp"
    )
    sfml_add_test(test-sfml-window "${WINDOW_SRC}" sfml-window)
    target_link_libraries(test-sfml-window PRIVATE sfml-test-config)
endif()

if(SFML_BUILD_GRAPHICS)
    SET(GRAPHICS_SRC
        "${SRCROOT}/Graphics/Rect.cpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.hpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.cpp"
    )
    sfml_add_test(test-sfml-graphics "${GRAPHICS_SRC}" sfml-graphics)
    target_link_libraries(test-sfml-graphics PRIVATE sfml-test-config)
endif()

if(SFML_BUILD_NETWORK)
    SET(NETWORK_SRC
        "${SRCROOT}/Network/Packet.cpp"
    )
    sfml_add_test(test-sfml-network "${NETWORK_SRC}" sfml-network)
    target_link_libraries(test-sfml-network PRIVATE sfml-test-config)
endif()

# Automatically run the tests at the end of the build
add_custom_target(runtests ALL
                  DEPENDS test-sfml-system test-sfml-window test-sfml-graphics test-sfml-network
)

add_custom_command(TARGET runtests
                   COMMENT "Run tests"
                   POST_BUILD COMMAND ctest ARGS --output-on-failure
)
