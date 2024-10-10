set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Audio)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Audio)

# all source files
set(SRC
    ${SRCROOT}/AudioResource.cpp
    ${INCROOT}/AudioResource.hpp
    ${SRCROOT}/AudioDevice.cpp
    ${SRCROOT}/AudioDevice.hpp
    ${INCROOT}/Export.hpp
    ${SRCROOT}/Listener.cpp
    ${INCROOT}/Listener.hpp
    ${SRCROOT}/Miniaudio.cpp
    ${SRCROOT}/MiniaudioUtils.hpp
    ${SRCROOT}/MiniaudioUtils.cpp
    ${SRCROOT}/Music.cpp
    ${INCROOT}/Music.hpp
    ${SRCROOT}/PlaybackDevice.cpp
    ${INCROOT}/PlaybackDevice.hpp
    ${SRCROOT}/Sound.cpp
    ${INCROOT}/Sound.hpp
    ${SRCROOT}/SoundBuffer.cpp
    ${INCROOT}/SoundBuffer.hpp
    ${SRCROOT}/SoundBufferRecorder.cpp
    ${INCROOT}/SoundBufferRecorder.hpp
    ${INCROOT}/SoundChannel.hpp
    ${SRCROOT}/InputSoundFile.cpp
    ${INCROOT}/InputSoundFile.hpp
    ${SRCROOT}/OutputSoundFile.cpp
    ${INCROOT}/OutputSoundFile.hpp
    ${SRCROOT}/SoundRecorder.cpp
    ${INCROOT}/SoundRecorder.hpp
    ${SRCROOT}/SoundSource.cpp
    ${INCROOT}/SoundSource.hpp
    ${SRCROOT}/SoundStream.cpp
    ${INCROOT}/SoundStream.hpp
)
source_group("" FILES ${SRC})

set(CODECS_SRC
    ${SRCROOT}/SoundFileFactory.cpp
    ${INCROOT}/SoundFileFactory.hpp
    ${INCROOT}/SoundFileFactory.inl
    ${INCROOT}/SoundFileReader.hpp
    ${SRCROOT}/SoundFileReaderFlac.hpp
    ${SRCROOT}/SoundFileReaderFlac.cpp
    ${SRCROOT}/SoundFileReaderMp3.hpp
    ${SRCROOT}/SoundFileReaderMp3.cpp
    ${SRCROOT}/SoundFileReaderOgg.hpp
    ${SRCROOT}/SoundFileReaderOgg.cpp
    ${SRCROOT}/SoundFileReaderWav.hpp
    ${SRCROOT}/SoundFileReaderWav.cpp
    ${INCROOT}/SoundFileWriter.hpp
    ${SRCROOT}/SoundFileWriterFlac.hpp
    ${SRCROOT}/SoundFileWriterFlac.cpp
    ${SRCROOT}/SoundFileWriterOgg.hpp
    ${SRCROOT}/SoundFileWriterOgg.cpp
    ${SRCROOT}/SoundFileWriterWav.hpp
    ${SRCROOT}/SoundFileWriterWav.cpp
)
source_group("codecs" FILES ${CODECS_SRC})

# Ensure certain files are compiled as Objective-C++
# See: https://miniaud.io/docs/manual/index.html#Building
if(SFML_OS_IOS)
    enable_language(OBJCXX)
    set_source_files_properties(${SRCROOT}/Miniaudio.cpp PROPERTIES LANGUAGE OBJCXX)
endif()

# let CMake know about our additional audio libraries paths (on Android and macOS)
if(SFML_OS_MACOS)
    list(APPEND CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/extlibs/libs-macos/Frameworks")
elseif(SFML_OS_ANDROID)
    list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/extlibs/android")
endif()

# find external libraries
find_package(Vorbis REQUIRED)
find_package(FLAC REQUIRED)

# define the sfml-audio target
sfml_add_library(Audio
                 SOURCES ${SRC} ${CODECS_SRC})

# avoids warnings in vorbisfile.h
target_compile_definitions(sfml-audio PRIVATE OV_EXCLUDE_STATIC_CALLBACKS FLAC__NO_DLL)

# disable miniaudio features we do not use
target_compile_definitions(sfml-audio PRIVATE MA_NO_MP3 MA_NO_FLAC MA_NO_ENCODING MA_NO_RESOURCE_MANAGER MA_NO_GENERATION)

# use standard fixed-width integer types
target_compile_definitions(sfml-audio PRIVATE MA_USE_STDINT)

# setup dependencies
target_link_libraries(sfml-audio
                      PUBLIC SFML::System
                      PRIVATE Vorbis::vorbis FLAC::FLAC)
if(SFML_OS_IOS)
    target_link_libraries(sfml-audio PRIVATE "-framework Foundation" "-framework CoreFoundation" "-framework CoreAudio" "-framework AudioToolbox" "-framework AVFoundation")
else()
    target_link_libraries(sfml-audio PRIVATE Vorbis::vorbisfile Vorbis::vorbisenc)
endif()

# miniaudio sources
target_include_directories(sfml-audio SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/miniaudio")

# minimp3 sources
target_include_directories(sfml-audio SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/minimp3")

if(SFML_OS_ANDROID)
    target_link_libraries(sfml-audio PRIVATE android OpenSLES)
endif()

if(SFML_OS_LINUX)
    target_link_libraries(sfml-audio PRIVATE dl)
endif()
