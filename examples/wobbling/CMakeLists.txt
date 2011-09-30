
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/wobbling)

# all source files
set(SRC ${SRCROOT}/Wobbling.cpp)

# define the shader target
sfml_add_example(wobbling GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics sfml-window sfml-system)
