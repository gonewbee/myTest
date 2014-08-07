LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	ExampleServer.cpp
LOCAL_C_INCLUDES := $(JNI_H_INCLUDE) \
	$(LOCAL_PATH)/../libExample
LOCAL_SHARED_LIBRARIES := \
	libutils libbinder libExampleServer
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := ExampleServer

include $(BUILD_EXECUTABLE)
