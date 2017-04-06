
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/clipboard)

# all source files
set(SRC ${SRCROOT}/Clipboard.cpp)

# define the clipboard target
sfml_add_example(clipboard
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics sfml-window sfml-system)