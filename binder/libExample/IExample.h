#ifndef _IEXAMPLE_H_
#define _IEXAMPLE_H_
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <utils/Errors.h>

using namespace android;
namespace zsy{
	#define ZSY_EXAMPLE_DESCRIPTOR "zsy_test_example"
	class IExample: public IInterface
	{
		public:
			enum{
				EXAMPLE_ADD100 = IBinder::FIRST_CALL_TRANSACTION,
				EXAMPLE_MULTIPLY100
			};
			DECLARE_META_INTERFACE(Example);
			virtual uint32_t add100(uint32_t num) = 0;
			virtual uint32_t multiply100(uint32_t num) = 0;	
	};
	
	class BnExample: public BnInterface<IExample>
	{
		public:
			virtual status_t onTransact(uint32_t code,
										const Parcel& data,
										Parcel* reply,
										uint32_t flags = 0);	
	};
};
#endif