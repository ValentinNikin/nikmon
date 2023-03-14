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
        auto existAgents = _databaseManager->getAgentsByIpAndMachineName(
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
    StatusResponse statusResponse;

    auto agentIt = std::find_if(_agents.cbegin(), _agents.cend(), [&statusRequest](const auto& agent) {
        return agent->id == statusRequest.id;
    });

    if (agentIt == _agents.cend()) {
        throw std::runtime_error("Agent is not registered");
    }

    auto agent = agentIt->get();

    // process commands confirmations
    for (const auto& c : statusRequest.confirmations) {
        auto agentWaitableCommandIt = std::find_if(agent->waitableCommands.cbegin(), agent->waitableCommands.cend(),
                                                 [&c](const auto& wc) {
            return wc->taskId == c.taskId && wc->type == c.type;
        });

        if (agentWaitableCommandIt == agent->waitableCommands.cend()) {
            // TODO: print something to log
            continue;
        }

        agent->waitableCommands.erase(agentWaitableCommandIt);
    }

    // process tasks items
    for (const auto& ti : statusRequest.items) {
        if (ti.status == TaskStatus::Error) {
            TaskItemErrorDB tieDB;
            tieDB.taskId = ti.id;
            tieDB.time = ti.time;
            tieDB.message = ti.errorMessage;
            _databaseManager->saveTaskItemError(tieDB);
            continue;
        }

        auto taskIt = std::find_if(agent->activeTasks.cbegin(), agent->activeTasks.cend(),
                                   [&ti](const auto& task) {
            return task->id == ti.id;
        });

        if (taskIt == agent->activeTasks.cend()) {
            // TODO: print something to log
            continue;
        }

        auto taskValueType = (*taskIt)->valueType;
        if (taskValueType == TaskValueType::uintType) {
            TaskItemDB<uint> tiDB;
            tiDB.taskId = ti.id;
            tiDB.time = ti.time;
            tiDB.value = atoi(ti.value.c_str());
            _databaseManager->saveTaskItem(tiDB);
        }
        else if (taskValueType == TaskValueType::floatType) {
            TaskItemDB<float> tiDB;
            tiDB.taskId = ti.id;
            tiDB.time = ti.time;
            tiDB.value = atof(ti.value.c_str());
            _databaseManager->saveTaskItem(tiDB);
        }
        else if (taskValueType == TaskValueType::textType) {
            TaskItemDB<std::string> tiDB;
            tiDB.taskId = ti.id;
            tiDB.time = ti.time;
            tiDB.value = ti.value;
            _databaseManager->saveTaskItem(tiDB);
        }
    }

    return statusResponse;
}