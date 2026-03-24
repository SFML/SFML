cmake_minimum_required(VERSION 3.22)
project(test-sfml-install CXX)

# This skips the find_package call when building via add_subdirectory since that will fail under those circumstances
if(PROJECT_IS_TOP_LEVEL)
    if(NOT BUILD_SHARED_LIBS)
        set(SFML_STATIC_LIBRARIES ON)
    endif()

    find_package(SFML 3.1.0 EXACT CONFIG REQUIRED COMPONENTS Graphics Network Audio)
endif()

add_executable(test-sfml-install Install.cpp)
target_link_libraries(test-sfml-install PRIVATE SFML::Graphics SFML::Network SFML::Audio)

if(SFML_USE_STATIC_STD_LIBS)
    set_target_properties(test-sfml-install PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

