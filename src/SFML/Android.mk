LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-system
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-system.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -llog -landroid

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-window
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-window.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -lEGL
LOCAL_SHARED_LIBRARIES := sfml-system

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-graphics
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-graphics.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
#LOCAL_EXPORT_LDLIBS := -lGLESv1_CM

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/extlibs/include
LOCAL_SHARED_LIBRARIES := freetype jpeg
LOCAL_SHARED_LIBRARIES += sfml-system sfml-window

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-audio
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-audio.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
#LOCAL_EXPORT_LDLIBS := -lOpenSLES
LOCAL_SHARED_LIBRARIES := openal sndfile
LOCAL_SHARED_LIBRARIES += sfml-system sfml-window

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
LOCAL_SHARED_LIBRARIES := sfml-system sfml-window

include $(PREBUILT_STATIC_LIBRARY)

$(call import-module,sfml/extlibs)
