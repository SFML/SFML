cmake_minimum_required(VERSION 3.24)

project(FindPackage LANGUAGES CXX)

set(SFML_STATIC_LIBRARIES ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)

add_executable(main ../main.cpp)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)
target_compile_features(main PRIVATE cxx_std_17)
