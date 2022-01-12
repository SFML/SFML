
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/sound)

# all source files
set(SRC ${SRCROOT}/Sound.cpp)

# define the sound target
sfml_add_example(sound
                 SOURCES ${SRC}
                 DEPENDS SFML::Audio
                 RESOURCES_DIR resources)
