#pragma once

#include "core/database/types/AgentDB.h"

struct Agent {
    std::string id;
    std::string machineName;
    std::string ip;
    int heartbeat;

    Agent(const AgentDB* agent)
        : id(agent->id),
        machineName(agent->machineName),
        ip(agent->ip),
        heartbeat(agent->heartbeat) {}
};