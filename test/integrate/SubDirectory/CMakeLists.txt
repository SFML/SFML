cmake_minimum_required(VERSION 3.24)

project(SubDirectory LANGUAGES CXX)

add_subdirectory(${CMAKE_SOURCE_DIR}/../../../ ${CMAKE_CURRENT_BINARY_DIR}/SFML EXCLUDE_FROM_ALL SYSTEM)

add_executable(main ../main.cpp)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)
target_compile_features(main PRIVATE cxx_std_17)
