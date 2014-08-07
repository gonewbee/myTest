#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include "libExampleClient.h"
#include "IExample.h"



using namespace android;
using namespace zsy;

static bool inited = false;
static sp<IExample> example = NULL;

#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, "libexample", __VA_ARGS__)

example_status_code_t example_init() {
	if (inited)
		return EXAMPLE_NO_ERROR;
	inited = true;
	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> binder = NULL;
	do {
		binder = sm->getService(String16(ZSY_EXAMPLE_DESCRIPTOR));
		if (binder != NULL)
			break;
		LOGI("waiting for exampleserver......");
		usleep(500000); //0.5s
	} while (true);
	example = interface_cast<IExample>(binder);
	return EXAMPLE_NO_ERROR;	
}

example_status_code_t example_term() {
	if (!inited)
		return EXAMPLE_NEED_INIT;
	inited = false;
	example.clear();
	example = NULL;
	return EXAMPLE_NO_ERROR;
}

uint32_t add100(uint32_t num) {
	LOGI("fortest add100 %s %d", __FILE__, __LINE__);	
	if (!inited) {
		LOGI("add100 %s %d need inited", __FILE__, __LINE__);	
	}
	return example->add100(num);	
}

uint32_t multiply100(uint32_t num) {
	LOGI("fortest multiply100 %s %d", __FILE__, __LINE__);	
	if (!inited) {
		LOGI("multiply100 %s %d need inited", __FILE__, __LINE__);	
	}
	return example->multiply100(num);	
}


