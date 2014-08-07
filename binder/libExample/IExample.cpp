#include <binder/Parcel.h>
#include <android/log.h>
#include <binder/Parcel.h>

#include "IExample.h"

using namespace android;
using namespace zsy;

#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, "libexample", __VA_ARGS__)

class BpExample: public BpInterface<IExample>
{
	public:
		BpExample(const sp<IBinder>& impl) : BpInterface<IExample>(impl){}
		uint32_t opentest();
		uint32_t closetest();
		uint32_t save2file(uint8_t *buf, uint32_t len, uint32_t *wLen);
		uint32_t getfromfile(uint8_t *buf, uint32_t len, uint32_t *rLen);
		uint32_t add100(uint32_t num);
		uint32_t multiply100(uint32_t num);	
};

uint32_t BpExample::opentest() {
	Parcel data, reply;
	data.writeInterfaceToken(IExample::getInterfaceDescriptor());
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	remote()->transact(EXAMPLE_OPENTEST, data, &reply);
	return reply.readInt32();
}

uint32_t BpExample::closetest() {
	Parcel data, reply;
	data.writeInterfaceToken(IExample::getInterfaceDescriptor());
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	remote()->transact(EXAMPLE_CLOSETEST, data, &reply);
	return reply.readInt32();
}

uint32_t BpExample::save2file(uint8_t * buf,uint32_t len,uint32_t * wLen) {
	Parcel data, reply;
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	data.writeInterfaceToken(IExample::getInterfaceDescriptor());
	data.writeInt32(len);
	data.write(buf, len);
	remote()->transact(EXAMPLE_SAVE2FILE, data, &reply);
	*wLen = reply.readInt32();
	return reply.readInt32();
}

uint32_t BpExample::getfromfile(uint8_t *buf, uint32_t len, uint32_t *rLen) {
	Parcel data, reply;
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	data.writeInterfaceToken(IExample::getInterfaceDescriptor());
	data.writeInt32(len);
	remote()->transact(EXAMPLE_GETFROMFILE, data, &reply);
	*rLen = reply.readInt32();
	reply.read(buf, *rLen);
	return reply.readInt32();
}

uint32_t BpExample::add100(uint32_t num) {
	Parcel data, reply;
	data.writeInterfaceToken(IExample::getInterfaceDescriptor());
	data.writeInt32(num);
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	remote()->transact(EXAMPLE_ADD100, data, &reply);
	return reply.readInt32();	
}

uint32_t BpExample::multiply100(uint32_t num) {
	Parcel data, reply;
	data.writeInterfaceToken(IExample::getInterfaceDescriptor());
	data.writeInt32(num);
	LOGI("fortest %s %d\n", __FILE__, __LINE__);
	remote()->transact(EXAMPLE_MULTIPLY100, data, &reply);
	return reply.readInt32();	
}

IMPLEMENT_META_INTERFACE(Example, ZSY_EXAMPLE_DESCRIPTOR);
status_t BnExample::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
	uint32_t ret = 0;
	switch(code){
	case EXAMPLE_OPENTEST: {
			CHECK_INTERFACE(IExample, data, reply);
			LOGI("fortest %s %d\n", __FILE__, __LINE__);
			ret = opentest();
			reply->writeInt32(ret);
		}
		break;
	case EXAMPLE_CLOSETEST: {
			CHECK_INTERFACE(IExample, data, reply);
			LOGI("fortest %s %d\n", __FILE__, __LINE__);
			ret = closetest();
			reply->writeInt32(ret);
		}
		break;
	case EXAMPLE_SAVE2FILE: {
			CHECK_INTERFACE(IExample, data, reply);
			LOGI("fortest %s %d\n", __FILE__, __LINE__);
			uint32_t len = data.readInt32();
			uint32_t wLen = 0;
			uint8_t *buf;
			buf = (uint8_t *)malloc(len);
			data.read(buf, len);
			ret = save2file(buf, len, &wLen );
			free(buf);
			reply->writeInt32(wLen);
			reply->writeInt32(ret);
		}
		break;
	case EXAMPLE_GETFROMFILE: {
			CHECK_INTERFACE(IExample, data, reply);
			LOGI("fortest %s %d\n", __FILE__, __LINE__);
			uint32_t len = data.readInt32();
			uint32_t rLen = 0;
			uint8_t *buf;
			buf = (uint8_t *)malloc(len);
			ret = getfromfile(buf, len, &rLen);
			reply->writeInt32(rLen);
			reply->write(buf, rLen);
			free(buf);
			reply->writeInt32(ret);
		}
		break;
	case EXAMPLE_ADD100:{
			CHECK_INTERFACE(IExample, data, reply);
			LOGI("fortest %s %d\n", __FILE__, __LINE__);
			uint32_t num = data.readInt32();
			ret = add100(num);
			reply->writeInt32(ret);	
		}
		break;
	case EXAMPLE_MULTIPLY100:{
			CHECK_INTERFACE(IExample, data, reply);
			LOGI("fortest %s %d\n", __FILE__, __LINE__);
			uint32_t num = data.readInt32();
			ret = multiply100(num);
			reply->writeInt32(ret);
		}
		break;
	default:
		return BBinder::onTransact(code, data, reply, flags);
	}
	return NO_ERROR;	
}
