
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

# link against threading library
if (NOT SFML_OS_IOS) # see https://gitlab.kitware.com/cmake/cmake/-/issues/18993
    find_package(Threads REQUIRED)
    target_link_libraries(island PRIVATE Threads::Threads)
endif()
