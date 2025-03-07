# all source files
set(SRC Stencil.cpp)

# define the stencil target
sfml_add_example(stencil GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics)
