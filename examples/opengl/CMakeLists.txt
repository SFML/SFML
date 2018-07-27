
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/opengl)

# all source files
set(SRC ${SRCROOT}/OpenGL.cpp)

# define the opengl target
sfml_add_example(opengl GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics OpenGL
                 RESOURCES_DIR resources)
