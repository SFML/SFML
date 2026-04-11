# all source files
set(SRC SoundEffects.cpp)
set(RESOURCES
    resources/doodle_pop.ogg
    resources/text-background.png
    resources/tuffy.ttf)

# define the sound-effects target
sfml_add_example(sound-effects GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Audio SFML::Graphics)
