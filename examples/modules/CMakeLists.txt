# all source files
set(SRC Modules.cpp)

# define the modules example target
sfml_add_example(modules-example GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics SFML::Modules)

target_link_libraries(modules-example PUBLIC sfml-modules)

add_dependencies(modules-example sfml-modules)
