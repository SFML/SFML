# all source files
set(SRC Keyboard.cpp)
set(RESOURCES
    resources/error_005.ogg
    resources/mouseclick1.ogg
    resources/mouserelease1.ogg
    resources/Tuffy.ttf)

# define the keyboard target
sfml_add_example(keyboard GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Audio SFML::Graphics)
