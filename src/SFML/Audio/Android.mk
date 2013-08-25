LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := sfml-audio

LOCAL_SRC_FILES := ALCheck.cpp
LOCAL_SRC_FILES += AudioDevice.cpp
LOCAL_SRC_FILES += Listener.cpp
LOCAL_SRC_FILES += Music.cpp
LOCAL_SRC_FILES += Sound.cpp
LOCAL_SRC_FILES += SoundBuffer.cpp
LOCAL_SRC_FILES += SoundBufferRecorder.cpp
LOCAL_SRC_FILES += SoundFile.cpp
LOCAL_SRC_FILES += SoundRecorder.cpp
LOCAL_SRC_FILES += SoundSource.cpp
LOCAL_SRC_FILES += SoundStream.cpp

include $(BUILD_STATIC_LIBRARY)

$(call import-module,extlibs)
