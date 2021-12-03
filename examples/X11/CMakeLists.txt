
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/X11)

# all source files
set(SRC ${SRCROOT}/X11.cpp)

# define the X11 target
sfml_add_example(X11Example GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-window X11)

# external dependency headers
target_include_directories(X11Example SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/X11)