
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/pong)

# all source files
set(SRC ${SRCROOT}/Pong.cpp)
if (SFML_OS_IOS)
    set(RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/ball.wav
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/sansation.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the pong target
sfml_add_example(pong GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS sfml-audio sfml-graphics
                 RESOURCES_DIR resources)
