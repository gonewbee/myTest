LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES:= \
    getso.c
LOCAL_LDLIBS := -llog

#编译生成libgetso.so
LOCAL_MODULE := libgetso
#编译动态库
include $(BUILD_SHARED_LIBRARY)
