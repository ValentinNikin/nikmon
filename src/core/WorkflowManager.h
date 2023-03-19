#pragma once

#include <vector>
#include <mutex>
#include <memory>

#include "IAgentCommunicator.h"
#include "IAgentsManager.h"

#include "core/database/IDatabaseManager.h"

#include "types/Agent.h"
#include "types/rest-api/EditTask.h"

class WorkflowManager : public IAgentCommunicator, public IAgentsManager {
public:
    WorkflowManager(const std::shared_ptr<IDatabaseManager>& databaseManager);

    // IAgentCommunicator implementation
    nikmon::types::RegistrationResponse registerAgent(const nikmon::types::RegistrationRequest&) override;
    nikmon::types::StatusResponse statusAgent(const nikmon::types::StatusRequest&) override;

    // IAgentsManager implementation
    std::vector<nikmon::types::AgentShortInfo> getAgents() override;
    std::string createAgent(const nikmon::types::EditAgent&) override;
    std::string assignTask(const std::string& agentId, const nikmon::types::EditTask& newTask) override;
    void editTask(const std::string& agentId, const nikmon::types::EditTask& editTask) override;
    void removeTask(const std::string& agentId, const std::string& taskId) override;
    void toggleTask(const std::string& agentId, const std::string& taskId) override;
    std::vector<nikmon::types::TaskShortInfo> getTasks(const std::string& agentId) override;

private:
    std::shared_ptr<IDatabaseManager> _databaseManager;


    using AgentsList = std::vector<std::unique_ptr<nikmon::types::Agent>>;
    std::mutex _mutex;
    AgentsList _agents;
    nikmon::types::Agent* findAgent(const std::string& id);
    void addAgent(std::unique_ptr<nikmon::types::Agent>);


//    ThreadSafeCollection<nikmon::types::Agent> _agents;
};