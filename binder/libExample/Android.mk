LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_PRELINK_MODULE := false
LOCAL_SRC_FILES:= \
	IExample.cpp


LOCAL_SHARED_LIBRARIES := \
	libutils \
	libbinder


LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libExampleS

include $(BUILD_SHARED_LIBRARY)
