cmake_minimum_required(VERSION 3.28)

# Check if GCC is being used and show warning
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(WARNING "C++20 modules not fully supported with GCC. SFML modules will not be built.")
    return()
endif()

# Create the module library first
add_library(sfml-modules)

# System module is always required
set(SFML_MODULES
    System.cppm
    SFML.cppm
)

# Add Window module files if enabled
if(SFML_BUILD_WINDOW)
    list(APPEND SFML_MODULES
        Window.cppm
    )
    target_compile_definitions(sfml-modules PRIVATE BUILD_WINDOW_MODULE)
endif()

# Add Graphics module files if enabled
if(SFML_BUILD_GRAPHICS)
    list(APPEND SFML_MODULES
        Graphics.cppm
    )
    target_compile_definitions(sfml-modules PRIVATE BUILD_GRAPHICS_MODULE)
endif()

# Add Audio module files if enabled
if(SFML_BUILD_AUDIO)
    list(APPEND SFML_MODULES
        Audio.cppm
    )
    target_compile_definitions(sfml-modules PRIVATE BUILD_AUDIO_MODULE)
endif()

# Add Network module files if enabled
if(SFML_BUILD_NETWORK)
    list(APPEND SFML_MODULES
        Network.cppm
    )
    target_compile_definitions(sfml-modules PRIVATE BUILD_NETWORK_MODULE)
endif()

# Add module sources directly
target_sources(sfml-modules PUBLIC FILE_SET CXX_MODULES FILES ${SFML_MODULES})

# System is always required
target_link_libraries(sfml-modules PUBLIC SFML::System)

# Conditionally link with other SFML libraries based on what's enabled
if(SFML_BUILD_WINDOW)
    target_link_libraries(sfml-modules PUBLIC SFML::Window)
endif()

if(SFML_BUILD_GRAPHICS)
    target_link_libraries(sfml-modules PUBLIC SFML::Graphics)
endif()

if(SFML_BUILD_AUDIO)
    target_link_libraries(sfml-modules PUBLIC SFML::Audio)
endif()

if(SFML_BUILD_NETWORK)
    target_link_libraries(sfml-modules PUBLIC SFML::Network)
endif()

# Set target properties
target_compile_features(sfml-modules PUBLIC cxx_std_20)

# Create an alias target
add_library(SFML::Modules ALIAS sfml-modules)

# Installation configuration
install(TARGETS sfml-modules
    EXPORT sfml-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SFML/modules
)
