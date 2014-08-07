#include <sys/types.h>  
#include <unistd.h>  
#include <grp.h>  
#include <binder/IPCThreadState.h>  
#include <binder/ProcessState.h>  
#include <binder/IServiceManager.h>  
#include <utils/Log.h>  
//#include <private/android_filesystem_config.h>  
#include "Example.h"

using namespace android;
using namespace zsy;

int main(int argc, char** argv){
	sp<ProcessState> proc((ProcessState::self()));
	sp<IServiceManager> sm = defaultServiceManager();
//	LOGI("ServiceManage: %p", sm.get());
	sm->addService(String16(ZSY_EXAMPLE_DESCRIPTOR), Example::instantiate());
//	sm->addService(String16(ZSY_EXAMPLE_DESCRIPTOR), new Example());
//	ExampleService::instantiate();
	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool();
	return 0;	
}
