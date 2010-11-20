
# define the sfml-main target
add_library(sfml-main STATIC ${CMAKE_SOURCE_DIR}/src/SFML/Main/SFML_Main.cpp)

# set the debug suffix
set_target_properties(sfml-main PROPERTIES DEBUG_POSTFIX -d)

# insert the major version number in the output filename
set_target_properties(sfml-main PROPERTIES OUTPUT_NAME "sfml-main")

# setup the install rule
install(TARGETS sfml-main ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel)
