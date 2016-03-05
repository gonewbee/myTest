LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := gles2jni
LOCAL_SRC_FILES  := com_example_zsy_hellogles_GLES2JNILib.c \
                    gles2_triangle.c

LOCAL_LDLIBS     := -llog -lGLESv2 -lEGL

include $(BUILD_SHARED_LIBRARY)
