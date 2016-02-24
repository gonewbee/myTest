//
// Created by zsy on 2/24/16.
//

#ifndef AGENTRDP_AGENTRDP_H_H
#define AGENTRDP_AGENTRDP_H_H

#include "freerdp/freerdp.h"

typedef struct agent_context {
    rdpContext context;
    BOOL disconnect;
    HANDLE capture_thread;
} agentContext;

#endif //AGENTRDP_AGENTRDP_H_H
