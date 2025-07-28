set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Graphics)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Graphics)

# all source files
set(SRC
    ${SRCROOT}/BlendMode.cpp
    ${INCROOT}/BlendMode.hpp
    ${INCROOT}/Color.hpp
    ${INCROOT}/Color.inl
    ${INCROOT}/CoordinateType.hpp
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
    ${SRCROOT}/StencilMode.cpp
    ${INCROOT}/StencilMode.hpp
    ${SRCROOT}/Texture.cpp
    ${INCROOT}/Texture.hpp
    ${SRCROOT}/TextureSaver.cpp
    ${SRCROOT}/TextureSaver.hpp
    ${SRCROOT}/Transform.cpp
    ${INCROOT}/Transform.hpp
    ${INCROOT}/Transform.inl
    ${SRCROOT}/Transformable.cpp
    ${INCROOT}/Transformable.hpp
    ${SRCROOT}/View.cpp
    ${INCROOT}/View.hpp
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
    ${SRCROOT}/RenderTextureImpl.hpp
    ${SRCROOT}/RenderTextureImplFBO.cpp
    ${SRCROOT}/RenderTextureImplFBO.hpp
    ${SRCROOT}/RenderTextureImplDefault.cpp
    ${SRCROOT}/RenderTextureImplDefault.hpp
)
source_group("render texture" FILES ${RENDER_TEXTURE_SRC})


# define the sfml-graphics target
sfml_add_library(Graphics
                 SOURCES ${SRC} ${DRAWABLES_SRC} ${RENDER_TEXTURE_SRC}
                 DEPENDENCIES "Dependencies.cmake.in")

# setup dependencies
target_link_libraries(sfml-graphics PUBLIC SFML::Window)

# stb_image sources
target_include_directories(sfml-graphics SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/stb_image")

# glad sources
target_include_directories(sfml-graphics SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")

# find external libraries
if(SFML_OS_ANDROID)
    target_link_libraries(sfml-graphics PRIVATE z)
elseif(SFML_OS_IOS)
    target_link_libraries(sfml-graphics PRIVATE z bz2)
endif()

if(SFML_USE_SYSTEM_DEPS)
    find_package(Freetype REQUIRED)
    find_package(HarfBuzz REQUIRED)
else()
    # use an immediately invoked function to scope option variables we have to set
    function(sfml_add_graphics_dependencies)
        include(FetchContent)

        # remember whether we are building SFML as a shared library
        if(BUILD_SHARED_LIBS)
            set(SFML_BUILD_SHARED_LIBS ON)
        endif()

        # The FreeType <-> HarfBuzz relationship is a complex one.
        # HarfBuzz relies on FreeType to manage glyph data.
        # FreeType relies on HarfBuzz for improved auto-hinting.
        # Using FetchContent alone will not work since both libraries want the
        # other to already exist as targets at configuration time.
        # We break this dependency cycle by patching the FreeType configuration
        # to assume that HarfBuzz exists but don't do any target linking itself.
        # We import HarfBuzz normally, telling it to link to FreeType.
        # After the HarfBuzz target has been created, we manually link the
        # FreeType target to HarfBuzz so the include directories and linker paths
        # to HarfBuzz are set for the FreeType library as well.

        set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
        set(CMAKE_DEBUG_POSTFIX d)
        set(BUILD_SHARED_LIBS OFF)
        set(SKIP_INSTALL_HEADERS ON)
        set(FT_DISABLE_ZLIB ON)
        set(FT_DISABLE_BZIP2 ON)
        set(FT_DISABLE_PNG ON)
        set(FT_DISABLE_HARFBUZZ ON)
        set(FT_DISABLE_BROTLI ON)
        set(FT_ENABLE_ERROR_STRINGS ON)

        FetchContent_Declare(Freetype
            GIT_REPOSITORY https://github.com/freetype/freetype.git
            GIT_TAG VER-2-13-2
            GIT_SHALLOW ON
            OVERRIDE_FIND_PACKAGE
            # patch out parts we don't want of the FreeType CMake configuration
            # - installing pkgconfig files
            # - assume HarfBuzz exists without linking targets yet
            PATCH_COMMAND ${CMAKE_COMMAND} -DFREETYPE_DIR=${FETCHCONTENT_BASE_DIR}/freetype-src -P ${PROJECT_SOURCE_DIR}/tools/freetype/PatchFreetype.cmake)

        set(HB_BUILD_SUBSET OFF)
        set(HB_HAVE_FREETYPE ON)
        set(HB_HAVE_CORETEXT OFF)

        FetchContent_Declare(HarfBuzz
            GIT_REPOSITORY https://github.com/harfbuzz/harfbuzz.git
            GIT_TAG 10.2.0
            GIT_SHALLOW ON
            OVERRIDE_FIND_PACKAGE
            # patch out parts we don't want of the HarfBuzz CMake configuration
            # - unnecessary warnings
            # - fix target_link_libraries being called without specifying PUBLIC
            PATCH_COMMAND ${CMAKE_COMMAND} -DHARFBUZZ_DIR=${FETCHCONTENT_BASE_DIR}/harfbuzz-src -P ${PROJECT_SOURCE_DIR}/tools/harfbuzz/PatchHarfBuzz.cmake)

        FetchContent_MakeAvailable(Freetype HarfBuzz)

        # add HarfBuzz preprocessor symbols
        target_compile_definitions(harfbuzz PRIVATE
            HB_DISABLE_DEPRECATED
            HB_NO_ATEXIT
            HB_NO_BORING_EXPANSION
            HB_NO_BUFFER_MESSAGE
            HB_NO_BUFFER_SERIALIZE
            HB_NO_BUFFER_VERIFY
            HB_NO_BITMAP
            HB_NO_DRAW
            HB_NO_ERRNO
            HB_NO_GETENV
            HB_NO_MT
            HB_NO_PAINT
            HB_NO_SETLOCALE
            HB_OPTIMIZE_SIZE
        )

        # Now that both targets exist, add the HarfBuzz include directories to the freetype target
        # We can't use target_link_libraries here because creating a circular dependency between FreeType and HarfBuzz
        # will make it impossible to resolve either of them with find_dependency in SFMLGraphicsDependencies.cmake
        # because we break the link here ourselves, we will have to re-establish it in Dependencies.cmake so CMake knows
        # it has to adjust the linker command line by duplicating the libraries that have circular dependencies
        target_include_directories(freetype SYSTEM PRIVATE $<TARGET_PROPERTY:harfbuzz,INTERFACE_INCLUDE_DIRECTORIES>)

        set_target_properties(freetype harfbuzz PROPERTIES FOLDER "Dependencies")

        # if building SFML as a shared library and linking our dependencies in
        # as static libraries we need to build them with -fPIC
        if(SFML_BUILD_SHARED_LIBS)
            set_target_properties(freetype harfbuzz PROPERTIES POSITION_INDEPENDENT_CODE ON)
        endif()

        # disable building dependencies as part of our unity build, they have their own internal unity build system
        set_target_properties(freetype harfbuzz PROPERTIES UNITY_BUILD OFF)

        sfml_set_stdlib(freetype)
        sfml_set_stdlib(harfbuzz)

        # HarfBuzz builds using a unity build system, all its files are combined into harfbuzz.cc
        # when building using mingw this can lead to "too many sections" and "file too big" errors
        # thus we will have to activate the -bigobj assembler flag when building using mingw
        if(WIN32 AND SFML_COMPILER_GCC)
            target_compile_options(harfbuzz PRIVATE -Wa,-mbig-obj)
        endif()

        add_library(Freetype::Freetype ALIAS freetype)
        add_library(HarfBuzz::HarfBuzz ALIAS harfbuzz)
    endfunction()
    sfml_add_graphics_dependencies()
endif()

target_link_libraries(sfml-graphics PRIVATE Freetype::Freetype HarfBuzz::HarfBuzz)

# we have to build SheenBidi ourselves in both cases since there is no system package for it
# use an immediately invoked function to scope option variables we have to set
function(sfml_add_sheenbidi_dependency)
    include(FetchContent)

    # remember whether we are building SFML as a shared library
    if(BUILD_SHARED_LIBS)
        set(SFML_BUILD_SHARED_LIBS ON)
    endif()

    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    set(BUILD_SHARED_LIBS OFF)

    FetchContent_Declare(SheenBidi
        GIT_REPOSITORY https://github.com/Tehreer/SheenBidi.git
        GIT_TAG v2.9.0
        GIT_SHALLOW ON
        # patch out parts we don't want of the SheenBidi CMake configuration
        # - disable installation
        # - turn the library into an object library so we can merge it into sfml-graphics
        PATCH_COMMAND ${CMAKE_COMMAND} -DSHEENBIDI_DIR=${FETCHCONTENT_BASE_DIR}/sheenbidi-src -P ${PROJECT_SOURCE_DIR}/tools/sheenbidi/PatchSheenBidi.cmake)
    FetchContent_MakeAvailable(SheenBidi)

    set_target_properties(SheenBidi PROPERTIES FOLDER "Dependencies")

    # if building SFML as a shared library and linking our dependencies in
    # as static libraries we need to build them with -fPIC
    if(SFML_BUILD_SHARED_LIBS)
        set_target_properties(SheenBidi PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()

    # disable building SheenBidi as part of our unity build, it has its own internal unity build system
    set_target_properties(SheenBidi PROPERTIES UNITY_BUILD OFF)

    sfml_set_stdlib(SheenBidi)

    # add SheenBidi object library and headers to our own target
    target_sources(sfml-graphics PRIVATE $<TARGET_OBJECTS:SheenBidi>)
    target_include_directories(sfml-graphics SYSTEM PRIVATE "${SheenBidi_SOURCE_DIR}/Headers")
endfunction()
sfml_add_sheenbidi_dependency()

# add preprocessor symbols
target_compile_definitions(sfml-graphics PRIVATE "STBI_FAILURE_USERMSG")

if(SFML_FATAL_OPENGL_ERRORS)
    target_compile_definitions(sfml-graphics PRIVATE "SFML_FATAL_OPENGL_ERRORS")
endif()

# Image.cpp must be compiled with the -fno-strict-aliasing
# when gcc is used; otherwise saving PNGs may crash in stb_image_write
if(SFML_COMPILER_GCC)
    set_source_files_properties(${SRCROOT}/Image.cpp PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
endif()
