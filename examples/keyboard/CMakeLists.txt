# all source files
set(SRC Keyboard.cpp)
if(SFML_OS_IOS)
    set(RESOURCES
        resources/error_005.ogg
        resources/mouseclick1.ogg
        resources/mouserelease1.ogg
        resources/Tuffy.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the keyboard target
sfml_add_example(keyboard GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Audio SFML::Graphics)
