# define the window target
sfml_add_example(window GUI_APP
                 DEPENDS SFML::Window)
target_sources(window PRIVATE Window.cpp)

# external dependency headers
target_include_directories(window SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/window)
