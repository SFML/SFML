
# include the SFML specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# let CMake know about our additional libraries paths
if (SFML_OS_WINDOWS)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers")
    if(SFML_COMPILER_GCC OR (SFML_COMPILER_CLANG AND MINGW))
        if(ARCH_X86)
            if(SFML_RUNTIME_UCRT)
                # taking precendence for provided libraries linking UCRT
                list(APPEND CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw-ucrt/x86")
            endif()
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw/x86")
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin/x86")
        elseif(ARCH_X64)
            if(SFML_RUNTIME_UCRT)
                # taking precendence for provided libraries linking UCRT
                list(APPEND CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw-ucrt/x64")
            endif()
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw/x64")
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin/x64")
        endif()
    elseif(SFML_COMPILER_MSVC OR (SFML_COMPILER_CLANG AND NOT MINGW))
        if(SFML_MSVC_VERSION LESS 14)
            if(ARCH_X86)
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc/x86")
            elseif(ARCH_X64)
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc/x64")
            endif()
        else()
            if(ARCH_ARM64)
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc-universal/ARM64")
            elseif(ARCH_X86)
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc-universal/x86")
            elseif(ARCH_X64)
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc-universal/x64")
            endif()
        endif()
    endif()
elseif(SFML_OS_MACOSX)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/lib/")
elseif(SFML_OS_IOS)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-ios/")
elseif(SFML_OS_ANDROID)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-android/${CMAKE_ANDROID_ARCH_ABI}")
endif()

# define the path of our additional CMake modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# set the output directory for SFML libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# add the modules subdirectories

# sfml-system
add_subdirectory(System)

# sfml-main and sfml-activity
if(SFML_OS_WINDOWS OR SFML_OS_ANDROID OR SFML_OS_IOS)
    add_subdirectory(Main)
endif()

# sfml-window
if(SFML_BUILD_WINDOW OR SFML_BUILD_GRAPHICS)
    add_subdirectory(Window)
endif()

# sfml-network
if(SFML_BUILD_NETWORK)
    add_subdirectory(Network)
endif()

# sfml-graphics
if(SFML_BUILD_GRAPHICS)
    add_subdirectory(Graphics)
endif()

# sfml-audio
if(SFML_BUILD_AUDIO)
    add_subdirectory(Audio)
endif()
