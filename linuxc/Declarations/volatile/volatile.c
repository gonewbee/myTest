/**
 * volatile关键字
 *
 * 优化编译：gcc -std=c99 -O volatile.c
 */
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
	clock_t t = clock();
	double d;
	// 修改非volatile变量100M次
	for (int n=0; n<10000; ++n) {
		for (int m=0; m<10000; ++m) {
			d += d*n*m;
		}
	}
	fprintf(stdout, "Modified a non-volatile variable 100M times."
			"Time used: %.2f seconds.\n",
			(double)(clock() - t)/CLOCKS_PER_SEC);

	// 修改volatile变量100M次
	t = clock();
	volatile double vd;
	for (int n=0; n<10000; ++n) {
		for (int m=0; m<10000; ++m) {
			vd += vd*n*m;
		}
	}
	fprintf(stdout, "Modified a volatile variable 100M times."
			"Time used: %.2f seconds.\n",
			(double)(clock() - t)/CLOCKS_PER_SEC);
	return 0;
}

