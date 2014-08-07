LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../android/frameworks/native/include \
	$(LOCAL_PATH)/../android/system/core/include

LOCAL_SRC_FILES:= \
	libExample/IExample.cpp

LOCAL_LDLIBS := \
	-L"$(LOCAL_PATH)/../android/libs" \
	-lbinder -lutils -lcutils


LOCAL_MODULE := libexample

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	libExampleService/Example.cpp

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../android/frameworks/native/include \
	$(LOCAL_PATH)/../android/system/core/include \
	$(LOCAL_PATH)/libExample

LOCAL_LDLIBS := \
	-L"$(LOCAL_PATH)/../android/libs" \
	-lbinder -lutils -lcutils \
	-L"$(LOCAL_PATH)/libs/$(APP_ABI)" \
	-lexample

LOCAL_MODULE := libexample_server
include $(BUILD_SHARED_LIBRARY)



include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	libExampleClient/libExampleClient.cpp

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../android/frameworks/native/include \
	$(LOCAL_PATH)/../android/system/core/include \
	$(LOCAL_PATH)/libExample

LOCAL_LDLIBS := \
	-L"$(LOCAL_PATH)/../android/libs" \
	-lbinder -lutils -lcutils \
	-L"$(LOCAL_PATH)/libs/$(APP_ABI)" \
	-lexample

LOCAL_MODULE := libexample_client
include $(BUILD_SHARED_LIBRARY)


##多模块编译的时候，编译顺序和名字有关？？
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	ExampleServer/ExampleServer.cpp

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../android/frameworks/native/include \
	$(LOCAL_PATH)/../android/system/core/include \
	$(LOCAL_PATH)/libExampleService \
	$(LOCAL_PATH)/libExample

LOCAL_LDLIBS := \
	-L"$(LOCAL_PATH)/../android/libs" \
	-lbinder -lutils -lcutils \
	-L"$(LOCAL_PATH)/libs/$(APP_ABI)" \
	-lexample_server

LOCAL_MODULE := texampleServer

include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	ExampleClient/ExampleClient.cpp

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../android/frameworks/native/include \
	$(LOCAL_PATH)/../android/system/core/include \
	$(LOCAL_PATH)/libExampleClient \
	$(LOCAL_PATH)/libExample

LOCAL_LDLIBS := \
	-L"$(LOCAL_PATH)/../android/libs" \
	-lbinder -lutils -lcutils \
	-L"$(LOCAL_PATH)/libs/$(APP_ABI)" \
	-lexample_client

LOCAL_MODULE := texampleClient

include $(BUILD_EXECUTABLE)



