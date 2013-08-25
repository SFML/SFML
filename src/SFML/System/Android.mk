LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := sfml-system

LOCAL_SRC_FILES := Clock.cpp
LOCAL_SRC_FILES += Err.cpp
LOCAL_SRC_FILES += Lock.cpp
LOCAL_SRC_FILES += Mutex.cpp
LOCAL_SRC_FILES += Sleep.cpp
LOCAL_SRC_FILES += String.cpp
LOCAL_SRC_FILES += Thread.cpp
LOCAL_SRC_FILES += ThreadLocal.cpp
LOCAL_SRC_FILES += Time.cpp
LOCAL_SRC_FILES += Unix/ClockImpl.cpp
LOCAL_SRC_FILES += Unix/MutexImpl.cpp
LOCAL_SRC_FILES += Unix/SleepImpl.cpp
LOCAL_SRC_FILES += Unix/ThreadImpl.cpp
LOCAL_SRC_FILES += Unix/ThreadLocalImpl.cpp

include $(BUILD_STATIC_LIBRARY)
