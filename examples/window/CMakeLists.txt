
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/window)

# all source files
set(SRC ${SRCROOT}/Window.cpp)

# find OpenGL and GLU
if(NOT IOS)
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    set(ADDITIONAL_LIBRARIES ${OPENGL_LIBRARIES})
else()
    set(ADDITIONAL_LIBRARIES -framework OpenGLES)
endif()

# define the window target
sfml_add_example(window GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-window sfml-system ${ADDITIONAL_LIBRARIES})
