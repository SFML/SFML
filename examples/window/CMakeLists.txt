
set(SRCROOT ${CMAKE_SOURCE_DIR}/examples/window)

# all source files
set(SRC ${SRCROOT}/Window.cpp)

# find OpenGL and GLU
find_package(OpenGL REQUIRED)

# define the window target
sfml_add_example(window GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-window sfml-system ${OPENGL_LIBRARIES})
