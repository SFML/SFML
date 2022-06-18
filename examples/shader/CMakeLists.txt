# all source files
set(SRC Effect.hpp
        Shader.cpp)

# define the shader target
sfml_add_example(shader GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
