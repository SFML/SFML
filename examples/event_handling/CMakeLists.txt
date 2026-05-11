# all source files
set(SRC EventHandling.cpp)
set(RESOURCES
    resources/tuffy.ttf)

# define the event_handling target
sfml_add_example(event_handling GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Graphics)
