# include the SFML specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# define the path of our additional CMake modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# set the output directory for SFML libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# add the modules subdirectories

# sfml-system
add_subdirectory(System)

# sfml-main
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

# on some platforms (e.g. Raspberry Pi 3 armhf), GCC requires linking libatomic to use <atomic> features
# that aren't supported by native CPU instructions (64-bit atomic operations on 32-bit architecture)
if(SFML_COMPILER_GCC)
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles("#include <atomic>
        int main(){std::atomic_ullong x(1); return x.fetch_add(1);}" ATOMIC_TEST)
    if(NOT ATOMIC_TEST)
        if(SFML_BUILD_GRAPHICS)
            target_link_libraries(sfml-graphics PRIVATE atomic)
        endif()
        if(SFML_BUILD_AUDIO)
            target_link_libraries(sfml-audio PRIVATE atomic)
        endif()
    endif()
endif()
