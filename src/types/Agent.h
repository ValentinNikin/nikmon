#pragma once

#include <vector>

#include "core/database/types/AgentDB.h"

#include "Task.h"
#include "Command.h"

namespace nikmon {
namespace types {

struct Agent {
    std::string id;
    std::string machineName;
    std::string ip;
    int heartbeat;

    std::vector<std::shared_ptr<Task>> tasks;
    std::mutex tasksMutex;

    std::vector<std::shared_ptr<Command>> waitableCommands;
    std::mutex waitableCommandsMutex;

    Agent(const AgentDB* agent)
            : id(agent->id),
              machineName(agent->machineName),
              ip(agent->ip),
              heartbeat(agent->heartbeat) {}
};

}
}

