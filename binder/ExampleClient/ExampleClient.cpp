#include <stdio.h>
#include "libExampleClient.h"

int main(int argc, char** argv)
{
	uint32_t num = 14;
	uint32_t ret = 0;
	example_init();
	ret = add100(num);
	printf("ret: %d\n", ret);
	ret = multiply100(num);
	printf("ret: %d\n", ret);
	return 0;
}
