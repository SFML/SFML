# define the joystick target
sfml_add_example(joystick GUI_APP
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
target_sources(joystick PRIVATE Joystick.cpp)
