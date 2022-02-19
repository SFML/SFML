# define the window target
sfml_add_example(vulkan GUI_APP
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
target_sources(vulkan PRIVATE Vulkan.cpp)

# external dependency headers
target_include_directories(vulkan SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/vulkan)
