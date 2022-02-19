# all source files
if (SFML_OS_IOS)
    set(RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/background.jpg
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/tuffy.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the opengl target
sfml_add_example(opengl GUI_APP
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
target_sources(opengl PRIVATE OpenGL.cpp)

# external dependency headers
target_include_directories(opengl SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/opengl)
