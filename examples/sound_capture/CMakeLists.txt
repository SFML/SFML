# define the sound-capture target
sfml_add_example(sound-capture
                 DEPENDS SFML::Audio)
target_sources(sound-capture PRIVATE SoundCapture.cpp)
