# define the X11 target
sfml_add_example(X11Example GUI_APP
                 DEPENDS SFML::Window X11)
target_sources(X11Example PRIVATE X11.cpp)

# external dependency headers
target_include_directories(X11Example SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/X11)
