
# define the sfml-main target
add_library(sfml-main STATIC ${CMAKE_SOURCE_DIR}/src/SFML/Main/SFML_Main.cpp)

# set the debug suffix
set_target_properties(sfml-main PROPERTIES DEBUG_POSTFIX -d)

# insert the major version number in the output filename
set_target_properties(sfml-main PROPERTIES OUTPUT_NAME "sfml${VERSION_MAJOR}-main")
