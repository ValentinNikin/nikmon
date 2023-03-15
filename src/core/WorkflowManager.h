#pragma once

#include <vector>
#include <mutex>
#include <memory>

#include "IAgentCommunicator.h"
#include "IAgentsManager.h"
#include "core/database/IDatabaseManager.h"

#include "types/Agent.h"
#include "types/EditTask.h"

class WorkflowManager : public IAgentCommunicator, public IAgentsManager {
    using RegistrationResponse = nikmon::types::RegistrationResponse;
    using RegistrationRequest = nikmon::types::RegistrationRequest;
    using StatusResponse = nikmon::types::StatusResponse;
    using StatusRequest = nikmon::types::StatusRequest;
    using Agent = nikmon::types::Agent;
    using EditTask = nikmon::types::EditTask;
public:
    WorkflowManager(const std::shared_ptr<IDatabaseManager>& databaseManager);

    RegistrationResponse registerAgent(const RegistrationRequest&) override;
    StatusResponse statusAgent(const StatusRequest&) override;

    void assignTask(const std::string& agentId, const EditTask& newTask) override;

private:
    std::shared_ptr<IDatabaseManager> _databaseManager;

    std::vector<std::shared_ptr<Agent>> _agents;
    std::mutex _agentsListMutex;
};