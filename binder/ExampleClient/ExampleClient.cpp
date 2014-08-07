#include <stdio.h>
#include "libExampleClient.h"

int main(int argc, char** argv)
{
	uint32_t num = 14;
	uint32_t ret = 0;
	uint8_t bufS[] = {'a', 'b', 'c', 'd', 'e', 'f'};
	uint8_t bufR[128] = {0};
	uint32_t len, tLen;
	example_init();
	ret = opentest();
	printf("ret: %d\n", ret);
	len = 5;
	ret = save2file(bufS, len,&tLen);
	printf("ret: %d;len:%d; tLen:%d\n", ret,len,  tLen);
	len = 32;
	ret = getfromfile(bufR, len,&tLen);
	printf("ret: %d;len:%d; tLen:%d\n", ret,len,  tLen);
	ret = closetest();
	printf("ret: %d\n", ret);
	ret = add100(num);
	printf("ret: %d\n", ret);
	ret = multiply100(num);
	printf("ret: %d\n", ret);
	return 0;
}
