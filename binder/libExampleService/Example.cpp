#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <utils/Errors.h>

#include "Example.h"

using namespace android;
using namespace zsy;

Example* Example::srv = NULL;
static FILE *fd = NULL;

#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, "libexample", __VA_ARGS__)
	
Example* Example::instantiate() {
	if (srv)
		return srv;
	srv = new Example();
	return srv;		
}

uint32_t Example::opentest() {
	fd = fopen("/data/system/test.txt", "a+");
	if (NULL == fd) {
		LOGI("open file error:%s\n", strerror(errno));
		return 0;
	}
	return 1;
}
uint32_t Example::closetest() {
	if (NULL!=fd)
		fclose(fd);
	return 1;
}

uint32_t Example::save2file(uint8_t * buf,uint32_t len, uint32_t *wLen) {
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	if (NULL==fd) {
		LOGI("file has not open\n");
		return 0;
	}
	fseek(fd, 0L, SEEK_END);
	int ret = fwrite(buf, 1, len, fd);
	if (ret<=0) {
		*wLen = 0;
		return 0;
	}
	*wLen = ret;
	return 1;
}

uint32_t Example::getfromfile(uint8_t * buf,uint32_t len,uint32_t * rLen) {
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	if (NULL==fd) {
		LOGI("file has not open\n");
		return 0;
	}
	fseek(fd, 0L, SEEK_SET);
	int ret = fread(buf, 1, len, fd);
	if (ret<=0) {
		*rLen = 0;
		return 0;
	}
	*rLen = ret;
	return 1;
}
uint32_t Example::add100(uint32_t num) {
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	return num+100;	
}

uint32_t Example::multiply100(uint32_t num) {
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	return num*100;			
}