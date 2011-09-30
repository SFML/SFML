
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/opengl)

# all source files
set(SRC ${SRCROOT}/OpenGL.cpp)

# find OpenGL and GLU
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# define the opengl target
sfml_add_example(opengl GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics sfml-window sfml-system ${OPENGL_LIBRARIES})
