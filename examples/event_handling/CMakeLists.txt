# all source files
set(SRC EventHandling.cpp)

# define the event_handling target
sfml_add_example(event_handling GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics)
