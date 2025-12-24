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
sfml_add_test(test-sfml-window "${WINDOW_SRC}" SFML::Window)