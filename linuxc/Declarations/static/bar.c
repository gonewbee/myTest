#include <stdio.h>
#include "bar.h"

int global_a = 11;
static int global_b = 22;

void print_global() {
    fprintf(stdout, "global_a:%d\n", global_a);
}

void set_static_value(int v) {
    global_b = v;
}

int get_static_value() {
    return global_b;
}

