# define the island target
sfml_add_example(island GUI_APP
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
target_sources(island PRIVATE Island.cpp)

# external dependency headers
target_include_directories(island SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/island)

# link against pthread
if(SFML_OS_LINUX OR SFML_OS_FREEBSD OR SFML_OS_MACOSX)
    target_link_libraries(island PRIVATE pthread)
endif()
