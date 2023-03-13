#include "WorkflowManager.h"

#include <algorithm>

WorkflowManager::WorkflowManager(
        const std::shared_ptr<IDatabaseManager>& databaseManager)
        : _databaseManager(databaseManager) {}

RegistrationResponse WorkflowManager::registerAgent(const RegistrationRequest& registrationRequest) {
    RegistrationResponse registrationResponse;

    auto activeAgentIt = std::find_if(_agents.cbegin(), _agents.cend(), [&registrationRequest](const auto& agent) {
        return agent->machineName == registrationRequest.machineName && agent->ip == registrationRequest.ip;
    });

    if (activeAgentIt != _agents.cend()) {
        registrationResponse.id = activeAgentIt->get()->id;
        registrationResponse.heartbeat = activeAgentIt->get()->heartbeat;
        // TODO: clear agent state
    }
    else {
        auto existAgents = _databaseManager->getAgents()->getByIpAndMachineName(
                registrationRequest.ip, registrationRequest.machineName);
        if (existAgents.empty()) {
            throw std::runtime_error("Not found configuration for the registered agent");
        }

        if (existAgents.size() > 1) {
            // TODO: print something to log, this is wrong situation
        }

        registrationResponse.id = existAgents[0]->id;
        registrationResponse.heartbeat = existAgents[0]->heartbeat;

        std::lock_guard<std::mutex> lg(_mutex);
        _agents.push_back(std::make_unique<Agent>(existAgents[0].get()));
    }

    return registrationResponse;
}

StatusResponse WorkflowManager::statusAgent(const StatusRequest& statusRequest) {
    (void)statusRequest;

    StatusResponse statusResponse;

    return statusResponse;
}