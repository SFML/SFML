
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/ios)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# All source files
set(SRC ${SRCROOT}/main.cpp)

# Use the resources from the android example
set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../android/assets/canary.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/../android/assets/image.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../android/assets/orchestral.ogg
    ${CMAKE_CURRENT_SOURCE_DIR}/../android/assets/sansation.ttf)

set_source_files_properties( ${RESOURCES} PROPERTIES
                              MACOSX_PACKAGE_LOCATION Resources )

# Define the window target
sfml_add_example(ios_demo GUI_APP
                 SOURCES ${SRC} ${RESOURCES}
                 DEPENDS sfml-window sfml-system sfml-graphics sfml-audio
                         "-framework OpenGLES")

# The app needs an identifier and signing to work correctly
SET_XCODE_PROPERTY(ios_demo CODE_SIGN_IDENTITY "iPhone Developer")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.sfml.ios_demo")
