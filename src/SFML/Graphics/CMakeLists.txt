
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Graphics)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Graphics)

# all source files
set(SRC
    ${SRCROOT}/Arial.hpp
    ${INCROOT}/BlendMode.hpp
    ${SRCROOT}/Color.cpp
    ${INCROOT}/Color.hpp
    ${INCROOT}/Drawable.hpp
    ${INCROOT}/Export.hpp
    ${SRCROOT}/Font.cpp
    ${INCROOT}/Font.hpp
    ${INCROOT}/Glyph.hpp
    ${SRCROOT}/GLCheck.cpp
    ${SRCROOT}/GLCheck.hpp
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
    ${SRCROOT}/RenderTextureImpl.cpp
    ${SRCROOT}/RenderTextureImpl.hpp
    ${SRCROOT}/RenderTextureImplFBO.cpp
    ${SRCROOT}/RenderTextureImplFBO.hpp
    ${SRCROOT}/RenderTextureImplDefault.cpp
    ${SRCROOT}/RenderTextureImplDefault.hpp
    ${SRCROOT}/RenderTarget.cpp
    ${INCROOT}/RenderTarget.hpp
    ${SRCROOT}/RenderWindow.cpp
    ${INCROOT}/RenderWindow.hpp
    ${SRCROOT}/Shader.cpp
    ${INCROOT}/Shader.hpp
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
    ${SRCROOT}/VertexArray.cpp
    ${INCROOT}/VertexArray.hpp
    ${SRCROOT}/stb_image/stb_image.h
    ${SRCROOT}/stb_image/stb_image_write.h
)

# let CMake know about our additional graphics libraries paths (on Windows and OSX)
if (WINDOWS OR MACOSX)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype")
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/jpeg")
endif()

# find external libraries
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(JPEG REQUIRED)
if(LINUX)
    find_package(X11 REQUIRED)
endif()
if(MACOSX)
    find_package(ZLIB REQUIRED)
endif()

# add include paths of external libraries
include_directories(${FREETYPE_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${JPEG_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

# build the list of libraries to link
# GL and X11 are only needed for shared build, as they are already linked by sfml-window
set(GRAPHICS_EXT_LIBS ${FREETYPE_LIBRARY} ${GLEW_LIBRARY} ${JPEG_LIBRARY})
if(BUILD_SHARED_LIBS)
    set(GRAPHICS_EXT_LIBS ${GRAPHICS_EXT_LIBS} ${OPENGL_gl_LIBRARY})
    if(LINUX)
        set(GRAPHICS_EXT_LIBS ${GRAPHICS_EXT_LIBS} ${X11_LIBRARIES})
    elseif(MACOSX)
        # We use static version of freetype so we need to link against zlib.
        set(GRAPHICS_EXT_LIBS ${GRAPHICS_EXT_LIBS} ${ZLIB_LIBRARIES})
    endif()
endif()

# add preprocessor symbols
add_definitions(-DGLEW_STATIC -DSTBI_FAILURE_USERMSG)

# ImageLoader.cpp must be compiled with the -fno-strict-aliasing
# when gcc is used; otherwise saving PNGs may crash in stb_image_write
if(COMPILER_GCC)
    set_source_files_properties(${SRCROOT}/ImageLoader.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
endif()

# define the sfml-graphics target
sfml_add_library(sfml-graphics
                 SOURCES ${SRC}
                 DEPENDS sfml-window sfml-system
                 EXTERNAL_LIBS ${GRAPHICS_EXT_LIBS})
