cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE SFML_MODULES *.cppm)

# Define configure_cpp_module_target function if not already defined
if(NOT COMMAND configure_cpp_module_target)
    function(configure_cpp_module_target target)
        if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
            target_sources(${target} PUBLIC FILE_SET CXX_MODULES FILES ${SFML_MODULES})
        else()
            message(WARNING "C++ modules require CMake 3.28+. Using standard compilation.")
            target_sources(${target} PRIVATE ${SFML_MODULES})
        endif()
    endfunction()
endif()

# Create the module library
add_library(sfml)

# Configure the module target
configure_cpp_module_target(sfml)

# Link with SFML libraries
target_link_libraries(sfml
    PUBLIC
        SFML::Audio 
        SFML::Graphics 
        SFML::Network 
        SFML::System 
        SFML::Window
)

# Set target properties
set_target_properties(sfml PROPERTIES 
    CXX_STANDARD 23 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF 
    CXX_MODULE_DYNDEP OFF
)

# Create an alias target
add_library(SFML::modules ALIAS sfml)

# Installation configuration
install(TARGETS sfml
    EXPORT sfml-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SFML/modules
)