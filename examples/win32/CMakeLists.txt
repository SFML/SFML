# all source files
set(SRC Win32.cpp)

# define the win32 target
sfml_add_example(win32 GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
