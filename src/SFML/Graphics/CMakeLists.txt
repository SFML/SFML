
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Graphics)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Graphics)

# all source files
set(SRC
    ${SRCROOT}/BlendMode.cpp
    ${INCROOT}/BlendMode.hpp
    ${SRCROOT}/Color.cpp
    ${INCROOT}/Color.hpp
    ${INCROOT}/Export.hpp
    ${SRCROOT}/Font.cpp
    ${INCROOT}/Font.hpp
    ${SRCROOT}/Glsl.cpp
    ${INCROOT}/Glsl.hpp
    ${INCROOT}/Glsl.inl
    ${INCROOT}/Glyph.hpp
    ${SRCROOT}/GLCheck.cpp
    ${SRCROOT}/GLCheck.hpp
    ${SRCROOT}/GLExtensions.hpp
    ${SRCROOT}/GLExtensions.cpp
    ${SRCROOT}/Image.cpp
    ${INCROOT}/Image.hpp
    ${SRCROOT}/ImageLoader.cpp
    ${SRCROOT}/ImageLoader.hpp
    ${INCROOT}/PrimitiveType.hpp
    ${INCROOT}/Rect.hpp
    ${INCROOT}/Rect.inl
    ${SRCROOT}/RenderStates.cpp
    ${INCROOT}/RenderStates.hpp
    ${SRCROOT}/RenderTexture.cpp
    ${INCROOT}/RenderTexture.hpp
    ${SRCROOT}/RenderTarget.cpp
    ${INCROOT}/RenderTarget.hpp
    ${SRCROOT}/RenderWindow.cpp
    ${INCROOT}/RenderWindow.hpp
    ${SRCROOT}/Shader.cpp
    ${INCROOT}/Shader.hpp
    ${SRCROOT}/Texture.cpp
    ${INCROOT}/Texture.hpp
    ${SRCROOT}/TextureSaver.cpp
    ${SRCROOT}/TextureSaver.hpp
    ${SRCROOT}/Transform.cpp
    ${INCROOT}/Transform.hpp
    ${SRCROOT}/Transformable.cpp
    ${INCROOT}/Transformable.hpp
    ${SRCROOT}/View.cpp
    ${INCROOT}/View.hpp
    ${SRCROOT}/Vertex.cpp
    ${INCROOT}/Vertex.hpp
)
source_group("" FILES ${SRC})

# drawables sources
set(DRAWABLES_SRC
    ${INCROOT}/Drawable.hpp
    ${SRCROOT}/Shape.cpp
    ${INCROOT}/Shape.hpp
    ${SRCROOT}/CircleShape.cpp
    ${INCROOT}/CircleShape.hpp
    ${SRCROOT}/RectangleShape.cpp
    ${INCROOT}/RectangleShape.hpp
    ${SRCROOT}/ConvexShape.cpp
    ${INCROOT}/ConvexShape.hpp
    ${SRCROOT}/Sprite.cpp
    ${INCROOT}/Sprite.hpp
    ${SRCROOT}/Text.cpp
    ${INCROOT}/Text.hpp
    ${SRCROOT}/VertexArray.cpp
    ${INCROOT}/VertexArray.hpp
    ${SRCROOT}/VertexBuffer.cpp
    ${INCROOT}/VertexBuffer.hpp
)
source_group("drawables" FILES ${DRAWABLES_SRC})

# render-texture sources
set(RENDER_TEXTURE_SRC
    ${SRCROOT}/RenderTextureImpl.cpp
    ${SRCROOT}/RenderTextureImpl.hpp
    ${SRCROOT}/RenderTextureImplFBO.cpp
    ${SRCROOT}/RenderTextureImplFBO.hpp
    ${SRCROOT}/RenderTextureImplDefault.cpp
    ${SRCROOT}/RenderTextureImplDefault.hpp
)
source_group("render texture" FILES ${RENDER_TEXTURE_SRC})


# define the sfml-graphics target
sfml_add_library(sfml-graphics
                 SOURCES ${SRC} ${DRAWABLES_SRC} ${RENDER_TEXTURE_SRC} ${STB_SRC})

# setup dependencies
target_link_libraries(sfml-graphics PUBLIC sfml-window)

# stb_image sources
target_include_directories(sfml-graphics PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/stb_image")

# glad sources
target_include_directories(sfml-graphics PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")

# let CMake know about our additional graphics libraries paths
if(SFML_OS_WINDOWS)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
elseif(SFML_OS_MACOSX)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/Frameworks")
elseif(SFML_OS_IOS)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
elseif(SFML_OS_ANDROID)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
endif()

# find external libraries
if(SFML_OS_ANDROID)
    target_link_libraries(sfml-graphics PRIVATE z)
elseif(SFML_OS_IOS)
    target_link_libraries(sfml-graphics PRIVATE z bz2)
endif()

sfml_find_package(Freetype INCLUDE "FREETYPE_INCLUDE_DIRS" LINK "FREETYPE_LIBRARY")
target_link_libraries(sfml-graphics PRIVATE Freetype)

# add preprocessor symbols
target_compile_definitions(sfml-graphics PRIVATE "STBI_FAILURE_USERMSG")

# ImageLoader.cpp must be compiled with the -fno-strict-aliasing
# when gcc is used; otherwise saving PNGs may crash in stb_image_write
if(SFML_COMPILER_GCC)
    set_source_files_properties(${SRCROOT}/ImageLoader.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
endif()
