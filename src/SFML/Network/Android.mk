LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := sfml-network

LOCAL_SRC_FILES := Ftp.cpp
LOCAL_SRC_FILES += Http.cpp
LOCAL_SRC_FILES += IpAddress.cpp
LOCAL_SRC_FILES += Packet.cpp
LOCAL_SRC_FILES += Socket.cpp
LOCAL_SRC_FILES += SocketSelector.cpp
LOCAL_SRC_FILES += TcpListener.cpp
LOCAL_SRC_FILES += TcpSocket.cpp
LOCAL_SRC_FILES += UdpSocket.cpp
LOCAL_SRC_FILES += Unix/SocketImpl.cpp

include $(BUILD_STATIC_LIBRARY)
