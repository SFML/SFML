set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/raw_input)

# all source files
set(SRC ${SRCROOT}/RawInput.cpp)

# define the window target
sfml_add_example(raw_input GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-window OpenGL)
