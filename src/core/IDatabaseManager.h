#pragma once

#include "database/repositories/IAgentsRepository.h"
#include "database/repositories/ITasksRepository.h"

class IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;

    virtual IAgentsRepository* getAgents() = 0;
    virtual ITasksRepository* getTasks() = 0;
};