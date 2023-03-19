#pragma once

#include <vector>
#include <memory>

#include "types/rest-api/TaskResultItem.h"
#include "types/rest-api/TaskErrorItem.h"
#include "types/rest-api/TaskInfo.h"

class ITasksManager {
public:
    virtual ~ITasksManager() = default;

    virtual std::unique_ptr<nikmon::types::TaskInfo> getTask(const std::string& taskId) = 0;
    virtual void removeTask(const std::string& taskId) = 0;
    virtual void removeTaskResults(const std::string& taskId) = 0;
    virtual std::vector<std::unique_ptr<nikmon::types::TaskResultItem>> getResults(const std::string& taskId, const long from, const long to) = 0;
    virtual std::vector<std::unique_ptr<nikmon::types::TaskErrorItem>> getErrors(const std::string& taskId, const long from, const long to) = 0;
};