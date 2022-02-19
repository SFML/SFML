# define the sound target
sfml_add_example(sound
                 DEPENDS SFML::Audio
                 RESOURCES_DIR resources)
target_sources(sound PRIVATE Sound.cpp)
