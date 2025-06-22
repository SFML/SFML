# all source files
set(SRC Text.cpp)

# define the text target
sfml_add_example(text GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics)
