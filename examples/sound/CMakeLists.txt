# all source files
set(SRC Sound.cpp)

# define the sound target
sfml_add_example(sound
                 SOURCES ${SRC}
                 DEPENDS SFML::Audio
                 RESOURCES_DIR resources)
