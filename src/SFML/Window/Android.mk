LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := sfml-window

LOCAL_SRC_FILES := Context.cpp
LOCAL_SRC_FILES += GlContext.cpp
LOCAL_SRC_FILES += GlResource.cpp
LOCAL_SRC_FILES += Joystick.cpp
LOCAL_SRC_FILES += JoystickManager.cpp
LOCAL_SRC_FILES += Keyboard.cpp
LOCAL_SRC_FILES += Mouse.cpp
LOCAL_SRC_FILES += VideoMode.cpp
LOCAL_SRC_FILES += Window.cpp
LOCAL_SRC_FILES += WindowImpl.cpp
LOCAL_SRC_FILES += EGLCheck.cpp
LOCAL_SRC_FILES += Android/WindowImplAndroid.cpp
LOCAL_SRC_FILES += Android/EglContext.cpp
LOCAL_SRC_FILES += Android/VideoModeImpl.cpp
LOCAL_SRC_FILES += Android/InputImpl.cpp
LOCAL_SRC_FILES += Android/JoystickImpl.cpp

include $(BUILD_STATIC_LIBRARY)
