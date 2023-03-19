#pragma once

#include <vector>
#include <memory>

#include "core/database/types/TaskDB.h"

class ITasksRepository {
public:
    virtual ~ITasksRepository() = default;

    virtual std::vector<std::unique_ptr<TaskDB>> list(const std::string& agentId, const bool onlyActive) = 0;
    virtual bool insert(TaskDB&) = 0;
    virtual std::unique_ptr<TaskDB> get(const std::string& taskId) = 0;
    virtual bool update(TaskDB&) = 0;
    virtual bool toggleState(const std::string& taskId, const bool isActive) = 0;
    virtual bool remove(const std::string& taskId) = 0;
};