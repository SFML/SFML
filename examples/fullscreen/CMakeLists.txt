
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/fullscreen)

# all source files
set(SRC ${SRCROOT}/Fullscreen.cpp)

# define the pong target
sfml_add_example(pong GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics sfml-window sfml-system)
