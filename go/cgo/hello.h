#ifndef _HELLO_H
#define _HELLO_H

typedef int (*pAdd)(int a, int b);
typedef int (*pSubtract)(int a, int b);
typedef int (*pMultiply)(int a, int b);
typedef int (*pDivide)(int a, int b);

typedef struct {
	pAdd add;
	pSubtract subtract;
	pMultiply multiply;
	pDivide divide;
} mathOps;

int func_add(int x, int y);

void mathOps_test(mathOps *math);

#endif