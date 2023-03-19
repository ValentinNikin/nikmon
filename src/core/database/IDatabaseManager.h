#pragma once

#include <vector>
#include <memory>

#include "core/database/types/AgentDB.h"
#include "core/database/types/TaskItemDB.h"
#include "core/database/types/TaskItemErrorDB.h"
#include "core/database/types/TaskDB.h"

#include "types/agent-communication/TaskItem.h"

#include "types/rest-api/EditAgent.h"
#include "types/rest-api/TaskResultItem.h"
#include "types/rest-api/TaskErrorItem.h"
#include "types/Task.h"

class IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;

    /// Agents
    virtual std::vector<std::unique_ptr<AgentDB>> getAgents() = 0;
    virtual std::vector<std::unique_ptr<AgentDB>> getAgentsByIpAndMachineName(
            const std::string ip, const std::string& machineName) = 0;
    virtual std::string createAgent(const nikmon::types::EditAgent&) = 0;

    /// Tasks items
    virtual void saveTaskItems(const std::vector<nikmon::types::TaskItem>& items) = 0;

    /// Tasks
    virtual std::string saveTask(const std::string& agentId, const nikmon::types::EditTask&) = 0;
    virtual std::vector<std::unique_ptr<TaskDB>> getTasks(const std::string& agentId, const bool onlyActive = false) = 0;
    virtual std::unique_ptr<TaskDB> getTask(const std::string& taskId) = 0;
    virtual void editTask(const nikmon::types::EditTask&) = 0;
    virtual void toggleTask(const std::string& taskId, const bool isActive) = 0;
    virtual void removeTask(const std::string& taskId) = 0;
    virtual void removeTaskResults(const std::string& taskId) = 0;
    virtual std::vector<std::unique_ptr<nikmon::types::TaskResultItem>> getResults(const std::string& taskId, const long from, const long to) = 0;
    virtual std::vector<std::unique_ptr<nikmon::types::TaskErrorItem>> getErrors(const std::string& taskId, const long from, const long to) = 0;
};