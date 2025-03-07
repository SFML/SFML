# all source files
set(SRC RawInput.cpp)

# define the raw_input target
sfml_add_example(raw_input GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics)
