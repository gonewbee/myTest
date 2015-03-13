#1. mk文件
##1.1 编译可执行文件

    LOCAL_PATH := $(call my-dir)
    
    include $(CLEAR_VARS)
    LOCAL_MODULE := my_test
    LOCAL_SRC_FILES := my_test.c
    LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
    LOCAL_LDLIBS := -L"$(LOCAL_PATH)/libs/armeabi-v7a" -ltest -llog
    include $(BUILD_EXECUTABLE)
##1.2 编译so库

    LOCAL_PATH := $(call my-dir)
    include $(CLEAR_VARS)
    LOCAL_MODULE := test
    
    LOCAL_SRC_FILES := \
            test.c \
            test2.c
            
    LOCAL_C_INCLUDES := \
            $(LOCAL_PATH)/include \
            $(LOCAL_PATH)/inc
    
    LOCAL_LDLIBS := \
            -L"$(LOCAL_PATH)" \
            -lmylib -llog
    
    include $(BUILD_SHARED_LIBRARY)
##1.3 执行编译命令
    ndk-build \
        NDK_PROJECT_PATH=`pwd` \
        APP_ABI=armeabi-v7a \
        APP_PLATFORM=android-14 \
        APP_BUILD_SCRIPT=`pwd`/XXX.mk \
        TARGET_PRODUCT=XXX \
        $@
#2 编写程序
头文件可以自己写，也可以通过javah XX.XX.XX命令生成。在处理数组时注意释放。

    jint Java_com_test_jni_set(JNIEnv* env, jobject thiz, jintArray buf) {
        jint *data = (*env)->GetIntArrayElements(env, buf, NULL);
        //...
        (*env)->ReleaseIntArrayElements(env, buf, data, 0);#注意释放
        return 1;
    }
    jint Java_com_test_jni_get(JNIEnv* env, jobject thiz, jintArray buf) {
        jint data[14] = {0};
        //...
        (*env)->SetIntArrayRegion(env, buf, 0, 14, (const jint *)data);
        return 1;
    }
    

