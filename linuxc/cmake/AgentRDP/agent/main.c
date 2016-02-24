#include <stdio.h>
#include "version.h"

int main(int argc, char *argv[]) {
    fprintf(stdout, "Hello Version:%d.%d\n", AGENTRDP_VERSION_MAJOR, AGENTRDP_VERSION_MINOR);
    return 0;
}
