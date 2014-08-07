LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	ExampleClient.cpp
LOCAL_C_INCLUDES := $(JNI_H_INCLUDE) \
	$(LOCAL_PATH)/../libExampleClient
LOCAL_SHARED_LIBRARIES := \
	libutils libbinder libExampleClient
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := ExampleClient

include $(BUILD_EXECUTABLE)
