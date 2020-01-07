MY_LOCAL_PATH := $(call my-dir)
LOCAL_PATH := $(MY_LOCAL_PATH)

include $(CLEAR_VARS)
LOCAL_MODULE	:= qrencode
LOCAL_SRC_FILES	:= ./libqrencode/$(TARGET_ARCH_ABI)/libqrencode.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE 	:= qrencode-android
LOCAL_CPPFLAGS 	:= -std=c++11
LOCAL_LDLIBS 	:= -llog -ljnigraphics -landroid
LOCAL_SHARED_LIBRARIES 	:= qrencode
LOCAL_C_INCLUDES := ./libqrencode/include/
LOCAL_SRC_FILES     := jni.cpp qrencode.cpp

include $(BUILD_SHARED_LIBRARY)