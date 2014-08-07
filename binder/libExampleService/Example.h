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
			uint32_t opentest();
			uint32_t closetest();
			uint32_t save2file(uint8_t *buf, uint32_t len, uint32_t *wLen);
			uint32_t getfromfile(uint8_t *buf, uint32_t len, uint32_t *rLen);
			uint32_t add100(uint32_t num);
			uint32_t multiply100(uint32_t num);	
		private:
			static Example* srv;
	};
};
#endif