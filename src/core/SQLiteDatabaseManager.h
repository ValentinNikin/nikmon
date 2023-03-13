#pragma once

#include "IDatabaseManager.h"

class SQLiteDatabaseManager : public IDatabaseManager {
public:
    SQLiteDatabaseManager();

    IAgentsRepository* getAgents() override;
    ITasksRepository* getTasks() override;
private:
    std::unique_ptr<IAgentsRepository> _agentsRepository;
    std::unique_ptr<ITasksRepository> _tasksRepository;
};