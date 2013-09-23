LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-system
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-system.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-window
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-window.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -lEGL -llog -landroid
LOCAL_SHARED_LIBRARIES := sfml-system

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-graphics
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-graphics.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES += sfml-system sfml-window

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-audio
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-audio.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := sfml-window sfml-system openal sndfile

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-network
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-network.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := sfml-system

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-main
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-main.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := sfml-window sfml-system

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-activity
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-activity.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(PREBUILT_SHARED_LIBRARY)

$(call import-module,sfml/extlibs)
