#pragma once

#include "ITasksManager.h"
#include "database/IDatabaseManager.h"

class TasksManager : public ITasksManager {
public:
    TasksManager(const std::shared_ptr<IDatabaseManager>& databaseManager);
public:
    std::unique_ptr<nikmon::types::TaskInfo> getTask(const std::string& taskId) override;
    void removeTask(const std::string& taskId) override;
    void removeTaskResults(const std::string& taskId) override;
    std::vector<std::unique_ptr<nikmon::types::TaskResultItem>> getResults(const std::string& taskId, const long from, const long to) override;
    std::vector<std::unique_ptr<nikmon::types::TaskErrorItem>> getErrors(const std::string& taskId, const long from, const long to) override;
private:
    std::shared_ptr<IDatabaseManager> _databaseManager;
};
