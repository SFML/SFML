# all source files
set(SRC Joystick.cpp)

# define the joystick target
sfml_add_example(joystick GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
