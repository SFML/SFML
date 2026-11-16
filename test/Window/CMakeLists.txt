sfml_add_test_utils(sfml-window-test-utils
    SOURCES ../TestUtilities/WindowUtil.hpp
            ../TestUtilities/WindowUtil.cpp
    DEPENDS SFML::Window sfml-system-test-utils)

sfml_set_option(SFML_RUN_DISPLAY_TESTS ON BOOL "ON to run tests that require a display, OFF to ignore them")
if(SFML_RUN_DISPLAY_TESTS)
    target_compile_definitions(sfml-window-test-utils PUBLIC SFML_RUN_DISPLAY_TESTS)
endif()

set(WINDOW_SRC
    Clipboard.test.cpp
    Context.test.cpp
    ContextSettings.test.cpp
    Event.test.cpp
    GlResource.test.cpp
    Joystick.test.cpp
    Mouse.test.cpp
    VideoMode.test.cpp
)

if(NOT SFML_OS_IOS)
    list(APPEND WINDOW_SRC
        # These modules aren't supported/relevant for iOS
        Cursor.test.cpp
        Keyboard.test.cpp
        Vulkan.test.cpp

        # At the time of writing these tests all pass except the window size ones
        Window.test.cpp
        WindowBase.test.cpp
    )
endif()
sfml_add_test(test-sfml-window SOURCES ${WINDOW_SRC} DEPENDS SFML::Window sfml-window-test-utils)
