# all source files
set(SRC SoundCapture.cpp)

# define the sound-capture target
sfml_add_example(sound-capture
                 SOURCES ${SRC}
                 DEPENDS SFML::Audio)
