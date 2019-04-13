
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/window)

# all source files
set(SRC ${SRCROOT}/Window.cpp)

if(SFML_OPENGL_ES)
    set(DEPENDENCY GLES) 
else()
    set(DEPENDENCY OpenGL) 
endif()

# define the window target
sfml_add_example(window GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-window ${DEPENDENCY})
