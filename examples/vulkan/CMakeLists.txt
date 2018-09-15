
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/vulkan)

# all source files
set(SRC ${SRCROOT}/Vulkan.cpp)

# define the window target
sfml_add_example(vulkan GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics
                 RESOURCES_DIR resources)
