
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/opengl)

# all source files
set(SRC ${SRCROOT}/OpenGL.cpp)

if (SFML_OS_IOS)
    set(RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/background.jpg
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/tuffy.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the opengl target
sfml_add_example(opengl GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)

# external dependency headers
target_include_directories(opengl SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/opengl)