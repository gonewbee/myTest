#include <stdio.h>
#include "version.h"
#include "freerdp/freerdp.h"
#include "agent_client.h"
#include "agentrdp.h"
#include "freerdp/client/cmdline.h"

int main(int argc, char *argv[]) {
    int status;
    rdpContext* context;
    rdpSettings* settings;
    RDP_CLIENT_ENTRY_POINTS clientEntryPoints;
    fprintf(stdout, "Hello Version:%d.%d\n", AGENTRDP_VERSION_MAJOR, AGENTRDP_VERSION_MINOR);
    memset(&clientEntryPoints, 0, sizeof(clientEntryPoints));
    clientEntryPoints.Size = sizeof(RDP_CLIENT_ENTRY_POINTS);
    clientEntryPoints.Version = RDP_CLIENT_INTERFACE_VERSION;
    RdpClientEntry(&clientEntryPoints);

    context = freerdp_client_context_new(&clientEntryPoints);
    if (!context)
        return 1;

    settings = context->settings;

    status = freerdp_client_settings_parse_command_line(context->settings, argc, argv, FALSE);

    status = freerdp_client_settings_command_line_status_print(settings, status, argc, argv);

    if (status) {
        freerdp_client_context_free(context);
        return 0;
    }

    agent_start_capture(context);
    freerdp_client_start(context);

    freerdp_client_context_free(context);
    return 0;
}
