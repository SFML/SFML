
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/vulkan)

# all source files
set(SRC ${SRCROOT}/Vulkan.cpp)

# define the window target
sfml_add_example(vulkan GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)

# external dependency headers
target_include_directories(vulkan SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/vulkan)