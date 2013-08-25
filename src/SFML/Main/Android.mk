LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := sfml-main
LOCAL_SRC_FILES := SFML_Main.cpp

include $(BUILD_STATIC_LIBRARY)
