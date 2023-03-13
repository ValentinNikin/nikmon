#pragma once

#include <vector>
#include <mutex>
#include <memory>

#include "IAgentCommunicator.h"
#include "IAgentsManager.h"
#include "IDatabaseManager.h"

#include "types/Agent.h"

class WorkflowManager : public IAgentCommunicator, public IAgentsManager {
public:
    WorkflowManager(const std::shared_ptr<IDatabaseManager>& databaseManager);

    RegistrationResponse registerAgent(const RegistrationRequest&) override;
    StatusResponse statusAgent(const StatusRequest&) override;

private:
    std::shared_ptr<IDatabaseManager> _databaseManager;
    std::vector<std::unique_ptr<Agent>> _agents;

    std::mutex _mutex;
};