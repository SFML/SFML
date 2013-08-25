LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := sfml-graphics

LOCAL_SRC_FILES := Color.cpp
LOCAL_SRC_FILES += GLCheck.cpp
LOCAL_SRC_FILES += GLExtensions.cpp
LOCAL_SRC_FILES += Image.cpp
LOCAL_SRC_FILES += ImageLoader.cpp
LOCAL_SRC_FILES += RenderStates.cpp
LOCAL_SRC_FILES += RenderTexture.cpp
LOCAL_SRC_FILES += RenderTarget.cpp
LOCAL_SRC_FILES += RenderWindow.cpp
LOCAL_SRC_FILES += Shader.cpp
LOCAL_SRC_FILES += Texture.cpp
LOCAL_SRC_FILES += TextureSaver.cpp
LOCAL_SRC_FILES += Transform.cpp
LOCAL_SRC_FILES += Transformable.cpp
LOCAL_SRC_FILES += View.cpp
LOCAL_SRC_FILES += Vertex.cpp
LOCAL_SRC_FILES += Shape.cpp
LOCAL_SRC_FILES += CircleShape.cpp
LOCAL_SRC_FILES += RectangleShape.cpp
LOCAL_SRC_FILES += ConvexShape.cpp
LOCAL_SRC_FILES += Sprite.cpp
LOCAL_SRC_FILES += Font.cpp
LOCAL_SRC_FILES += Text.cpp
LOCAL_SRC_FILES += VertexArray.cpp
LOCAL_SRC_FILES += RenderTextureImpl.cpp
LOCAL_SRC_FILES += RenderTextureImplFBO.cpp
LOCAL_SRC_FILES += RenderTextureImplDefault.cpp

LOCAL_CPPFLAGS := -DGL_GLEXT_PROTOTYPES -DSFML_OPENGL_ES

include $(BUILD_STATIC_LIBRARY)

$(call import-module,extlibs)
