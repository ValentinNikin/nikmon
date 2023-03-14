#pragma once

#include <vector>
#include <memory>

#include "core/database/types/TaskItemErrorDB.h"

class ITasksItemsErrorsRepository {
public:
    virtual ~ITasksItemsErrorsRepository() = default;

    virtual std::vector<std::unique_ptr<TaskItemErrorDB>> list(const std::string& taskId) = 0;
    virtual std::vector<std::unique_ptr<TaskItemErrorDB>> list(const std::string& taskId, long int from, long int to) = 0;
    virtual bool insert(TaskItemErrorDB&) = 0;
    virtual bool remove(const std::string& taskId) = 0;
};