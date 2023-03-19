#pragma once

#include <vector>

#include "types/rest-api/EditAgent.h"
#include "types/rest-api/EditTask.h"
#include "types/rest-api/TaskShortInfo.h"
#include "types/rest-api/AgentShortInfo.h"

class IAgentsManager {
public:
    virtual ~IAgentsManager() = default;

    virtual std::vector<nikmon::types::AgentShortInfo> getAgents() = 0;
    virtual void createAgent(const nikmon::types::EditAgent&) = 0;
    virtual void assignTask(const std::string& agentId, const nikmon::types::EditTask&) = 0;
    virtual void editTask(const std::string& agentId, const nikmon::types::EditTask& editTask) = 0;
    virtual void removeTask(const std::string& agentId, const std::string& taskId) = 0;
    virtual void toggleTask(const std::string& agentId, const std::string& taskId) = 0;
    virtual std::vector<nikmon::types::TaskShortInfo> getTasks(const std::string& agentId) = 0;
};