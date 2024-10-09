# help Vorbis find the Ogg::Ogg target that FetchContent provides
file(READ "${VORBIS_DIR}/CMakeLists.txt" VORBIS_CMAKELISTS_CONTENTS)
string(REPLACE "find_package(Ogg REQUIRED)" "set(OGG_LIBRARIES Ogg::ogg)" VORBIS_CMAKELISTS_CONTENTS "${VORBIS_CMAKELISTS_CONTENTS}")
string(REPLACE "\n\nadd_subdirectory(lib)" "\nset(CMAKE_DEBUG_POSTFIX d)\nadd_subdirectory(lib)" VORBIS_CMAKELISTS_CONTENTS "${VORBIS_CMAKELISTS_CONTENTS}")
file(WRITE "${VORBIS_DIR}/CMakeLists.txt" "${VORBIS_CMAKELISTS_CONTENTS}")
