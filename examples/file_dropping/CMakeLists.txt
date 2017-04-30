
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/file_dropping)

# all source files
set(SRC ${SRCROOT}/FileDropping.cpp)

# define the file_dropping target
sfml_add_example(file_dropping GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics sfml-window sfml-system)
