# all source files
set(SRC SoundEffects.cpp)
if(SFML_OS_IOS)
    set(RESOURCES
        resources/doodle_pop.ogg
        resources/text-background.png
        resources/tuffy.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the sound-effects target
sfml_add_example(sound-effects GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Audio SFML::Graphics
                 RESOURCES_DIR resources)
