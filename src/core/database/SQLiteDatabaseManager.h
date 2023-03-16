#pragma once

#include "IDatabaseManager.h"

#include "core/database/repositories/ITasksItemsRepository.h"
#include "core/database/repositories/IAgentsRepository.h"
#include "core/database/repositories/ITasksRepository.h"
#include "core/database/repositories/ITasksItemsErrorsRepository.h"

#include "types/rest-api/EditAgent.h"

class SQLiteDatabaseManager : public IDatabaseManager {
public:
    SQLiteDatabaseManager();

    std::vector<std::unique_ptr<AgentDB>> getAgents() override;
    std::vector<std::unique_ptr<AgentDB>> getAgentsByIpAndMachineName(const std::string ip, const std::string& machineName) override;
    void createAgent(const nikmon::types::EditAgent& editAgent) override;

    void saveTaskItem(const TaskItemDB<uint>& taskItem) override;
    void saveTaskItem(const TaskItemDB<float>& taskItem) override;
    void saveTaskItem(const TaskItemDB<std::string>& taskItem) override;
    void saveTaskItemError(const TaskItemErrorDB& taskItemError) override;

    void saveTask(const nikmon::types::Task& task) override;
    std::vector<std::unique_ptr<TaskDB>> getTasks(const std::string& agentId) override;
    std::unique_ptr<TaskDB> getTask(const std::string& taskId) override;
    void editTask(const nikmon::types::EditTask& editTask) override;
    void toggleTask(const std::string& taskId, const TaskStatus newState) override;
    void removeTask(const std::string& taskId) override;
    void removeTaskResults(const std::string& taskId) override;
    std::vector<std::unique_ptr<nikmon::types::TaskResultItem>> getResults(const std::string& taskId, const long from, const long to) override;
    std::vector<std::unique_ptr<nikmon::types::TaskErrorItem>> getErrors(const std::string& taskId, const long from, const long to) override;
private:
    std::unique_ptr<IAgentsRepository> _agentsRepository;
    std::unique_ptr<ITasksRepository> _tasksRepository;
    std::unique_ptr<ITasksItemsRepository<uint>> _tasksItemsRepositoryUint;
    std::unique_ptr<ITasksItemsRepository<float>> _tasksItemsRepositoryFloat;
    std::unique_ptr<ITasksItemsRepository<std::string>> _tasksItemsRepositoryText;
    std::unique_ptr<ITasksItemsErrorsRepository> _tasksItemsErrorsRepository;
};