# all source files
set(SRC Batching.cpp)
if (SFML_OS_IOS)
    set(RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex0.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex1.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex2.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex3.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex4.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex5.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex6.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex7.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tex8.png)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the sprite batch demo target
sfml_add_example(sprite_batch GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Graphics
                 RESOURCES_DIR resources)
