cmake_minimum_required(VERSION 3.22)
project(sfml-example CXX)

set(SFML_DIR "${CMAKE_ANDROID_NDK}/sources/third_party/sfml/lib/${CMAKE_ANDROID_ARCH_ABI}/cmake/SFML")

# Create the libsfml-example.so library that contains the application's c++ code
add_library(sfml-example SHARED main.cpp)

# Find and link SFML
find_package(SFML 3 REQUIRED COMPONENTS Graphics)
target_link_libraries(sfml-example PRIVATE SFML::Graphics android log)

# The ANativeActivity_onCreate function from SFML::Main needs to be exposed in
# our libsfml-example.so file so that is can be loaded at runtime.
target_link_libraries(sfml-example PUBLIC
  -Wl,--whole-archive
  SFML::Main
  -Wl,--no-whole-archive
)
