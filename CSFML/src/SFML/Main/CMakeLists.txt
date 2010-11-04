
# define the csfml-main target
add_library(csfml-main STATIC ${CMAKE_SOURCE_DIR}/src/SFML/Main/SFML_Main.cpp)

# set the debug suffix
set_target_properties(csfml-main PROPERTIES DEBUG_POSTFIX -d)

# insert the major version number in the output filename
set_target_properties(csfml-main PROPERTIES OUTPUT_NAME "csfml-main")
