#include <stdio.h>
#include "version.h"
#include "freerdp/freerdp.h"

int main(int argc, char *argv[]) {
    RDP_CLIENT_ENTRY_POINTS clientEntryPoints;
    fprintf(stdout, "Hello Version:%d.%d\n", AGENTRDP_VERSION_MAJOR, AGENTRDP_VERSION_MINOR);
    memset(&clientEntryPoints, 0, sizeof(clientEntryPoints));
    fprintf(stdout, "size:%lu\n", sizeof(clientEntryPoints));
    return 0;
}
