# all source files
set(SRC Joystick.cpp)

if(SFML_OS_IOS)
    set(RESOURCES resources/tuffy.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the joystick target
sfml_add_example(joystick GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Graphics)
