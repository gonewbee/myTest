
LOCAL_PATH := $(call my-dir)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	Example.cpp
LOCAL_C_INCLUDES := $(JNI_H_INCLUDE) \
	$(LOCAL_PATH)/../libExample
LOCAL_SHARED_LIBRARIES := \
	libutils libbinder libExampleS
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libExampleServer

include $(BUILD_SHARED_LIBRARY)
