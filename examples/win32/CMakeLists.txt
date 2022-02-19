# define the win32 target
sfml_add_example(win32 GUI_APP
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
target_sources(win32 PRIVATE Win32.cpp)
