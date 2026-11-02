set(AUDIO_SRC
    AudioResource.test.cpp
    InputSoundFile.test.cpp
    Listener.test.cpp
    Music.test.cpp
    OutputSoundFile.test.cpp
    Sound.test.cpp
    SoundBuffer.test.cpp
    SoundBufferRecorder.test.cpp
    SoundFileFactory.test.cpp
    SoundFileReader.test.cpp
    SoundFileWriter.test.cpp
    SoundRecorder.test.cpp
    SoundSource.test.cpp
    SoundStream.test.cpp
)
sfml_add_test(test-sfml-audio SOURCES ${AUDIO_SRC} DEPENDS SFML::Audio sfml-audio-test-utils)

if(SFML_OS_IOS)
    set(AUDIO_RESOURCE
        ding-🐌.flac
        ding-🐌.mp3
        ding-ń.flac
        ding-ń.mp3
        ding.flac
        ding.mp3
        doodle_pop-🐌.ogg
        doodle_pop-ń.ogg
        doodle_pop.ogg
        killdeer-🐌.wav
        killdeer-ń.wav
        killdeer.wav
    )
    target_sources(test-sfml-audio PRIVATE ${AUDIO_RESOURCE})
    set_source_files_properties(${AUDIO_RESOURCE} PROPERTIES MACOSX_PACKAGE_LOCATION "Audio")
endif()

