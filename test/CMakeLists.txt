set(SRCROOT "${PROJECT_SOURCE_DIR}/test")

add_library(sfml-test-main STATIC "${SRCROOT}/DoctestMain.cpp")
target_include_directories(sfml-test-main PUBLIC "${PROJECT_SOURCE_DIR}/extlibs/headers" "${SRCROOT}/TestUtilities")
target_compile_features(sfml-test-main PRIVATE cxx_std_17)

# System is always built
SET(SYSTEM_SRC
    "${SRCROOT}/System/Clock.cpp"
    "${SRCROOT}/System/FileInputStream.cpp"
    "${SRCROOT}/System/Time.cpp"
    "${SRCROOT}/System/Vector2.cpp"
    "${SRCROOT}/System/Vector3.cpp"
    "${SRCROOT}/TestUtilities/SystemUtil.hpp"
    "${SRCROOT}/TestUtilities/SystemUtil.cpp"
)
sfml_add_test(test-sfml-system "${SYSTEM_SRC}" SFML::System)
target_link_libraries(test-sfml-system PRIVATE sfml-test-main)

if(SFML_BUILD_WINDOW)
    SET(WINDOW_SRC
        "${SRCROOT}/Window/ContextSettings.cpp"
        "${SRCROOT}/Window/VideoMode.cpp"
        "${SRCROOT}/TestUtilities/WindowUtil.hpp"
        "${SRCROOT}/TestUtilities/WindowUtil.cpp"
    )
    sfml_add_test(test-sfml-window "${WINDOW_SRC}" SFML::Window)
    target_link_libraries(test-sfml-window PRIVATE sfml-test-main)
endif()

if(SFML_BUILD_GRAPHICS)
    SET(GRAPHICS_SRC
        "${SRCROOT}/Graphics/BlendMode.cpp"
        "${SRCROOT}/Graphics/Color.cpp"
        "${SRCROOT}/Graphics/Rect.cpp"
        "${SRCROOT}/Graphics/Transform.cpp"
        "${SRCROOT}/Graphics/Vertex.cpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.hpp"
        "${SRCROOT}/TestUtilities/GraphicsUtil.cpp"
    )
    sfml_add_test(test-sfml-graphics "${GRAPHICS_SRC}" SFML::Graphics)
    target_link_libraries(test-sfml-graphics PRIVATE sfml-test-main)
endif()

if(SFML_BUILD_NETWORK)
    SET(NETWORK_SRC
        "${SRCROOT}/Network/Packet.cpp"
    )
    sfml_add_test(test-sfml-network "${NETWORK_SRC}" SFML::Network)
    target_link_libraries(test-sfml-network PRIVATE sfml-test-main)
endif()

# Automatically run the tests at the end of the build
add_custom_target(runtests ALL
                  DEPENDS test-sfml-system test-sfml-window test-sfml-graphics test-sfml-network
)

add_custom_command(TARGET runtests
                   COMMENT "Run tests"
                   POST_BUILD COMMAND "${CMAKE_CTEST_COMMAND}" --output-on-failure -C $<CONFIG>
)
