cmake_minimum_required(VERSION 3.22)
project(sfml-example CXX)

set(SFML_DIR "${CMAKE_ANDROID_NDK}/sources/third_party/sfml/lib/${CMAKE_ANDROID_ARCH_ABI}/cmake/SFML")

# Create the libsfml-example.so library that contains the application's c++ code
add_library(sfml-example SHARED main.cpp)

# Find and link SFML.
# We must link to the network and audio components even if we don't use them, because sfml-activity will try to load them
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network)
target_link_libraries(sfml-example PRIVATE SFML::Graphics SFML::Audio SFML::Network SFML::Activity android log)

# The ANativeActivity_onCreate function from SFML::Main needs to be exposed in
# our libsfml-example.so file so that is can be loaded at runtime by sfml-activity.
target_link_libraries(sfml-example PUBLIC
  -Wl,--whole-archive
  SFML::Main
  -Wl,--no-whole-archive
)

# We need to manually copy the OpenAL library into the apk, even if we don't use it, because sfml-activity will try to load it
add_library(OpenAL::OpenAL SHARED IMPORTED)
set_target_properties(OpenAL::OpenAL PROPERTIES IMPORTED_LOCATION "${CMAKE_ANDROID_NDK}/sources/third_party/sfml/extlibs/lib/${CMAKE_ANDROID_ARCH_ABI}/libopenal.so")
target_link_libraries(sfml-example PRIVATE OpenAL::OpenAL)
