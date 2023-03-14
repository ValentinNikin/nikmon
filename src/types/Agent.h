#pragma once

#include <vector>

#include "core/database/types/AgentDB.h"

#include "Task.h"
#include "Command.h"

struct Agent {
    std::string id;
    std::string machineName;
    std::string ip;
    int heartbeat;

    std::vector<std::unique_ptr<Task>> activeTasks;
    std::vector<std::unique_ptr<Command>> waitableCommands;

    Agent(const AgentDB* agent)
        : id(agent->id),
        machineName(agent->machineName),
        ip(agent->ip),
        heartbeat(agent->heartbeat) {}
};