#pragma once

#include <vector>
#include <memory>

#include "core/database/types/AgentDB.h"
#include "core/database/types/TaskItemDB.h"
#include "core/database/types/TaskItemErrorDB.h"
#include "types/Task.h"

class IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;

    /// Agents
    virtual std::vector<std::unique_ptr<AgentDB>> getAgentsByIpAndMachineName(
            const std::string ip, const std::string& machineName) = 0;

    /// Tasks items
    virtual void saveTaskItem(const TaskItemDB<uint>&) = 0;
    virtual void saveTaskItem(const TaskItemDB<float>&) = 0;
    virtual void saveTaskItem(const TaskItemDB<std::string>&) = 0;
    virtual void saveTaskItemError(const TaskItemErrorDB&) = 0;

    /// Tasks
    virtual void saveTask(const nikmon::types::Task&) = 0;
};