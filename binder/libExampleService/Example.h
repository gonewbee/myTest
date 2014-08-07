#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <binder/IInterface.h>
#include <binder/Binder.h>
#include "IExample.h"

using namespace android;
namespace zsy {
	class Example: public BnExample {
		public:
			static Example* instantiate();
			uint32_t add100(uint32_t num);
			uint32_t multiply100(uint32_t num);	
		private:
			static Example* srv;
	};
};
#endif