# define the shader target
sfml_add_example(shader GUI_APP
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
target_sources(shader PRIVATE Shader.cpp)
