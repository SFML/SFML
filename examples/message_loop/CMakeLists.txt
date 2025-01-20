# all source files
set(SRC MessageLoop.cpp)

# define the message_loop target
sfml_add_example(message_loop GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics)
