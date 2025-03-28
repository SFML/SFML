cmake_minimum_required(VERSION 3.28)

# System module is always required
set(SFML_MODULES
    Config.cppm
    System.cppm
)

# Add Window module files if enabled
if(SFML_BUILD_WINDOW)
    list(APPEND SFML_MODULES
        Window.cppm
    )
endif()

# Add Graphics module files if enabled
if(SFML_BUILD_GRAPHICS)
    list(APPEND SFML_MODULES
        Graphics.cppm
    )
endif()

# Add Audio module files if enabled
if(SFML_BUILD_AUDIO)
    list(APPEND SFML_MODULES
        Audio.cppm
    )
endif()

# Add Network module files if enabled
if(SFML_BUILD_NETWORK)
    list(APPEND SFML_MODULES
        Network.cppm
    )
endif()

# Define configure_cpp_module_target function if not already defined
if(NOT COMMAND configure_cpp_module_target)
    function(configure_cpp_module_target target)
        target_sources(${target} PUBLIC FILE_SET CXX_MODULES FILES ${SFML_MODULES})
    endfunction()
endif()

# Create the module library
add_library(sfml)

# Configure the module target
configure_cpp_module_target(sfml)

# System is always required
target_link_libraries(sfml PUBLIC SFML::System)

# Conditionally link with other SFML libraries based on what's enabled
if(SFML_BUILD_WINDOW)
    target_link_libraries(sfml PUBLIC SFML::Window)
endif()

if(SFML_BUILD_GRAPHICS)
    target_link_libraries(sfml PUBLIC SFML::Graphics)
endif()

if(SFML_BUILD_AUDIO)
    target_link_libraries(sfml PUBLIC SFML::Audio)
endif()

if(SFML_BUILD_NETWORK)
    target_link_libraries(sfml PUBLIC SFML::Network)
endif()

# Set target properties
target_compile_features(sfml PUBLIC cxx_std_20)

# Create an alias target
add_library(SFML::modules ALIAS sfml)

# Installation configuration
install(TARGETS sfml
    EXPORT sfml-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SFML/modules
)