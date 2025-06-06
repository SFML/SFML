find_package(Threads REQUIRED)

# all source files
set(SRC SoundDevice.cpp)

# define the sound-device target
sfml_add_example(sound-device
                 SOURCES ${SRC}
                 DEPENDS SFML::Audio)

target_link_libraries(sound-device PRIVATE Threads::Threads)
