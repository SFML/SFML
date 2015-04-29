# all source files
set(SRC Tennis.cpp)
if(SFML_OS_IOS)
    set(RESOURCES
        resources/ball.wav
        resources/tuffy.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the tennis target
sfml_add_example(tennis GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Audio SFML::Graphics
                 RESOURCES_DIR resources)
