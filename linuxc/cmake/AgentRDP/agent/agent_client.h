//
// Created by zsy on 2/24/16.
//

#ifndef AGENTRDP_AGENT_CLIENT_H
#define AGENTRDP_AGENT_CLIENT_H

#include "freerdp/freerdp.h"

int RdpClientEntry(RDP_CLIENT_ENTRY_POINTS* pEntryPoints);
int agent_start_capture(rdpContext* context);

#endif //AGENTRDP_AGENT_CLIENT_H
