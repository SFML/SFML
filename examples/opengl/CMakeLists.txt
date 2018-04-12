
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/opengl)

# all source files
set(SRC ${SRCROOT}/OpenGL.cpp)

if (SFML_OS_IOS)
    set(RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/background.jpg
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/texture.jpg
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/sansation.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the opengl target
sfml_add_example(opengl GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS sfml-graphics OpenGL
                 RESOURCES_DIR resources)
