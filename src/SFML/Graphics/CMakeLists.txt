# define the sfml-graphics target
sfml_add_library(Graphics)

# all source files
target_sources(sfml-graphics PRIVATE
    BlendMode.cpp
    Font.cpp
    Glsl.cpp
    GLCheck.cpp
    GLExtensions.cpp
    Image.cpp
    ImageLoader.cpp
    RenderStates.cpp
    RenderTexture.cpp
    RenderTarget.cpp
    RenderWindow.cpp
    Shader.cpp
    Texture.cpp
    TextureSaver.cpp
    Transform.cpp
    Transformable.cpp
    View.cpp
)

# drawables sources
target_sources(sfml-graphics PRIVATE
    Shape.cpp
    CircleShape.cpp
    RectangleShape.cpp
    ConvexShape.cpp
    Sprite.cpp
    Text.cpp
    VertexArray.cpp
    VertexBuffer.cpp
)

# render-texture sources
target_sources(sfml-graphics PRIVATE
    RenderTextureImpl.cpp
    RenderTextureImplFBO.cpp
    RenderTextureImplDefault.cpp
)

# setup dependencies
target_link_libraries(sfml-graphics PUBLIC SFML::Window)

# stb_image sources
target_include_directories(sfml-graphics SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/stb_image")

# glad sources
target_include_directories(sfml-graphics SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")

# let CMake know about our additional graphics libraries paths
if(SFML_OS_WINDOWS)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
elseif(SFML_OS_MACOSX)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
    list(APPEND CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/Frameworks")
elseif(SFML_OS_IOS)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
elseif(SFML_OS_ANDROID)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/freetype2")
endif()

# find external libraries
if(SFML_OS_ANDROID)
    target_link_libraries(sfml-graphics PRIVATE z)
elseif(SFML_OS_IOS)
    target_link_libraries(sfml-graphics PRIVATE z bz2)
endif()

# starting from Visual Studio 2015, inline versions of some C functions are used; for compatibility link this library
# see https://docs.microsoft.com/en-us/cpp/porting/overview-of-potential-upgrade-issues-visual-cpp?view=msvc-160#libraries
if(SFML_COMPILER_MSVC OR (SFML_COMPILER_CLANG AND SFML_OS_WINDOWS AND NOT MINGW))
    target_link_libraries(sfml-graphics PRIVATE legacy_stdio_definitions.lib)
endif()

sfml_find_package(Freetype INCLUDE "FREETYPE_INCLUDE_DIRS" LINK "FREETYPE_LIBRARY")
target_link_libraries(sfml-graphics PRIVATE Freetype)

# add preprocessor symbols
target_compile_definitions(sfml-graphics PRIVATE "STBI_FAILURE_USERMSG")

# ImageLoader.cpp must be compiled with the -fno-strict-aliasing
# when gcc is used; otherwise saving PNGs may crash in stb_image_write
if(SFML_COMPILER_GCC)
    set_source_files_properties(ImageLoader.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
endif()
