
set(INCROOT ${CMAKE_SOURCE_DIR}/include/SFML/Graphics)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/SFML/Graphics)

# all source files
set(SRC
    ${INCROOT}/BlendMode.h
    ${SRCROOT}/Color.cpp
    ${INCROOT}/Color.h
    ${SRCROOT}/Font.cpp
    ${SRCROOT}/FontStruct.h
    ${INCROOT}/Font.h
    ${INCROOT}/Glyph.h
    ${SRCROOT}/Image.cpp
    ${SRCROOT}/ImageStruct.h
    ${INCROOT}/Image.h
    ${SRCROOT}/Rect.cpp
    ${INCROOT}/Rect.h
    ${SRCROOT}/RenderImage.cpp
    ${SRCROOT}/RenderImageStruct.h
    ${INCROOT}/RenderImage.h
    ${SRCROOT}/RenderWindow.cpp
    ${SRCROOT}/RenderWindowStruct.h
    ${INCROOT}/RenderWindow.h
    ${SRCROOT}/Shader.cpp
    ${SRCROOT}/ShaderStruct.h
    ${INCROOT}/Shader.h
    ${SRCROOT}/Shape.cpp
    ${SRCROOT}/ShapeStruct.h
    ${INCROOT}/Shape.h
    ${SRCROOT}/Sprite.cpp
    ${SRCROOT}/SpriteStruct.h
    ${INCROOT}/Sprite.h
    ${SRCROOT}/Text.cpp
    ${SRCROOT}/TextStruct.h
    ${INCROOT}/Text.h
    ${INCROOT}/Types.h
    ${SRCROOT}/View.cpp
    ${SRCROOT}/ViewStruct.h
    ${INCROOT}/View.h
)

# define the csfml-graphics target
csfml_add_library(csfml-graphics
                  SOURCES ${SRC}
                  DEPENDS optimized ${SFML_GRAPHICS_LIBRARY} debug ${SFML_GRAPHICS_LIBRARY_DEBUG}
                          optimized ${SFML_WINDOW_LIBRARY} debug ${SFML_WINDOW_LIBRARY_DEBUG}
                          optimized ${SFML_SYSTEM_LIBRARY} debug ${SFML_SYSTEM_LIBRARY_DEBUG})
