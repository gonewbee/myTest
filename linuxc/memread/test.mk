LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := memread
LOCAL_SRC_FILES := memread.c
LOCAL_LDLIBS :=  -llog
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := test
LOCAL_SRC_FILES := test.c
include $(BUILD_EXECUTABLE)
