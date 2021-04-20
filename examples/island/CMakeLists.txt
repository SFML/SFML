
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/island)

# all source files
set(SRC ${SRCROOT}/Island.cpp)

# define the island target
sfml_add_example(island GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics
                 RESOURCES_DIR resources)

# external dependency headers
target_include_directories(island SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/island)