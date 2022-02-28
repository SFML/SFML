# define the sfml-audio target
sfml_add_library(Audio)

# all source files
target_sources(sfml-audio PRIVATE
    ALCheck.cpp
    AlResource.cpp
    AudioDevice.cpp
    Listener.cpp
    Music.cpp
    Sound.cpp
    SoundBuffer.cpp
    SoundBufferRecorder.cpp
    InputSoundFile.cpp
    OutputSoundFile.cpp
    SoundRecorder.cpp
    SoundSource.cpp
    SoundStream.cpp
)

target_sources(sfml-audio PRIVATE
    SoundFileFactory.cpp
    SoundFileReaderFlac.cpp
    SoundFileReaderMp3.cpp
    SoundFileReaderOgg.cpp
    SoundFileReaderWav.cpp
    SoundFileWriterFlac.cpp
    SoundFileWriterOgg.cpp
    SoundFileWriterWav.cpp
)

# let CMake know about our additional audio libraries paths (on Windows and OSX)
if(SFML_OS_WINDOWS)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/AL")
elseif(SFML_OS_MACOSX)
    list(APPEND CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/Frameworks")
elseif(SFML_OS_ANDROID)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/headers/AL")
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/android")
endif()

# find external libraries
sfml_find_package(OpenAL INCLUDE "OPENAL_INCLUDE_DIR" LINK "OPENAL_LIBRARY")
sfml_find_package(VORBIS INCLUDE "VORBIS_INCLUDE_DIRS" LINK "VORBIS_LIBRARIES")
sfml_find_package(FLAC INCLUDE "FLAC_INCLUDE_DIR" LINK "FLAC_LIBRARY")

# avoids warnings in vorbisfile.h
target_compile_definitions(VORBIS INTERFACE "OV_EXCLUDE_STATIC_CALLBACKS")
target_compile_definitions(FLAC INTERFACE "FLAC__NO_DLL")

# setup dependencies
target_link_libraries(sfml-audio PRIVATE OpenAL)

# minimp3 sources
target_include_directories(sfml-audio SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/minimp3")

if(SFML_OS_ANDROID)
    target_link_libraries(sfml-audio PRIVATE android OpenSLES)
endif()

target_link_libraries(sfml-audio
                      PUBLIC SFML::System
                      PRIVATE VORBIS FLAC)
