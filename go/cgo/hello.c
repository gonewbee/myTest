#include <stdio.h>
#include "hello.h"

int func_add(int x, int y) {
	fprintf(stdout, "enter c function\n");
	return x+y;
}

void mathOps_test(mathOps *math) {
	int a = 1100;
	int b = 11;
	int ret = 0;

	ret = math->add(a, b);
	fprintf(stdout, "%d + %d = %d\n", a, b, ret);
	ret = math->subtract(a, b);
	fprintf(stdout, "%d - %d = %d\n", a, b, ret);
	ret = math->multiply(a, b);
	fprintf(stdout, "%d * %d = %d\n", a, b, ret);
	ret = math->divide(a, b);
	fprintf(stdout, "%d / %d = %d\n", a, b, ret);
}
