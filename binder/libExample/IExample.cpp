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
		uint32_t add100(uint32_t num);
		uint32_t multiply100(uint32_t num);	
};

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
