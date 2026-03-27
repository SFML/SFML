# all source files
set(SRC OpenGL.cpp)
set(RESOURCES
    resources/background.jpg
    resources/tuffy.ttf)

# define the opengl target
sfml_add_example(opengl GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Graphics)

# external dependency headers
target_include_directories(opengl SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/include)
