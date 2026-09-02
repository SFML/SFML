# all source files
set(SRC Tennis.cpp)
set(RESOURCES
    resources/ball.wav
    resources/sfml_logo.png
    resources/tuffy.ttf)

# define the tennis target
sfml_add_example(tennis GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Audio SFML::Graphics)
