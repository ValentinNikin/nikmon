#pragma once

#include <string>

struct AgentDB {
    std::string id;
    std::string machineName;
    std::string ip;
    int heartbeat;
    std::string info;

    AgentDB() = default;

    AgentDB(const AgentDB& otherAgent) {
        id = otherAgent.id;
        machineName = otherAgent.machineName;
        ip = otherAgent.ip;
        heartbeat = otherAgent.heartbeat;
        info = otherAgent.info;
    }
};