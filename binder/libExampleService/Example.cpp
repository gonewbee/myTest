#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <utils/Errors.h>

#include "Example.h"

using namespace android;
using namespace zsy;

Example* Example::srv = NULL;

#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, "libexample", __VA_ARGS__)
	
Example* Example::instantiate() {
	if (srv)
		return srv;
	srv = new Example();
	return srv;		
}

uint32_t Example::add100(uint32_t num) {
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	return num+100;	
}

uint32_t Example::multiply100(uint32_t num) {
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	return num*100;			
}