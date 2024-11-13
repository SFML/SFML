cmake_minimum_required(VERSION 3.24)

project(FetchContent LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../../
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main ../main.cpp)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)
target_compile_features(main PRIVATE cxx_std_17)
