
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/iOS)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# All source files
set(SRC ${SRCROOT}/main.cpp)

set(RESOURCES
    ${SRCROOT}/resources/canary.wav
    ${SRCROOT}/resources/image.png
    ${SRCROOT}/resources/orchestral.ogg
    ${SRCROOT}/resources/sansation.ttf)

set_source_files_properties( ${RESOURCES} PROPERTIES
                              MACOSX_PACKAGE_LOCATION Resources )

# Define the window target
sfml_add_example(ios_demo GUI_APP
                 SOURCES ${SRC} ${RESOURCES}
                 DEPENDS sfml-window sfml-system sfml-graphics sfml-audio
                         "-framework OpenGLES")

# The app needs an identifier and signing to work correctly
sfml_set_xcode_property(ios_demo CODE_SIGN_IDENTITY "iPhone Developer")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.sfml.ios_demo")
