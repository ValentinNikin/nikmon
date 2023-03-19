#include "WorkflowManager.h"

#include <algorithm>

#include "types/Task.h"
#include "types/enums/TaskResultStatus.h"
#include "types/enums/CommandType.h"

using namespace nikmon::types;

WorkflowManager::WorkflowManager(
        const std::shared_ptr<IDatabaseManager>& databaseManager)
        : _databaseManager(databaseManager) {}

RegistrationResponse WorkflowManager::registerAgent(const RegistrationRequest& registrationRequest) {

    // extract agent configuration from database
    auto agentsDb = _databaseManager->getAgentsByIpAndMachineName(registrationRequest.ip, registrationRequest.machineName);
    if (agentsDb.empty()) {
        throw std::runtime_error("Not found configuration for the registered agent");
    }

    if (agentsDb.size() > 1) {
        // TODO: print something to log, this is wrong situation
    }

    const auto& agentDb = agentsDb[0];

    // try to find already exist agent in active agents
    auto agent = findAgent(agentDb->id);
    if (agent != nullptr) {
        // if agent has been found we should reset agent state
        agent->reset();
    }
    else {
        auto newAgent = std::make_unique<Agent>(agentDb.get());
        newAgent->init(_databaseManager->getTasks(newAgent->id, true));
        addAgent(std::move(newAgent));

        agent = newAgent.get();
    }

    RegistrationResponse registrationResponse;

    registrationResponse.id = agent->id;
    registrationResponse.heartbeat = agent->heartbeat;

    return registrationResponse;
}

StatusResponse WorkflowManager::statusAgent(const StatusRequest& statusRequest) {
    auto agent = findAgent(statusRequest.id);
    if (agent == nullptr) {
        throw std::runtime_error("Agent is not registered");
    }

    _databaseManager->saveTaskItems(statusRequest.items);

    return agent->prepareStatusResponse(statusRequest.confirmations);
}

std::vector<AgentShortInfo> WorkflowManager::getAgents() {
    std::vector<AgentShortInfo> agents;
    auto agentsDb = _databaseManager->getAgents();

    for (const auto& a : agentsDb) {
        AgentShortInfo agentSI;
        agentSI.id = a->id;
        agentSI.ip = a->ip;
        agentSI.machineName = a->machineName;
        agentSI.status = findAgent(a->id) != nullptr ? AgentStatus::connected : AgentStatus::disconnected;

        agents.push_back(agentSI);
    }

    return agents;
}

void WorkflowManager::createAgent(const nikmon::types::EditAgent& editAgent) {
    _databaseManager->createAgent(editAgent);
}

void WorkflowManager::assignTask(const std::string& agentId, const EditTask& newTask) {
    auto agent = findAgent(agentId);
    if (agent != nullptr) {
        agent->assignTask(newTask);
    }

    _databaseManager->saveTask(agentId, newTask);
}

std::vector<TaskShortInfo> WorkflowManager::getTasks(const std::string& agentId) {
    std::vector<TaskShortInfo> results;

    auto agent = findAgent(agentId);

    auto tasksDb = _databaseManager->getTasks(agentId);
    for (const auto& t : tasksDb) {
        TaskShortInfo taskSI;
        taskSI.id = t->id;
        taskSI.key = t->key;
        taskSI.status = TaskStatus::Stopped;

        if (agent != nullptr) {
            auto processingTask = agent->getTask(t->id);
            if (processingTask != nullptr) {
                taskSI.status = processingTask->getStatus();
            }
        }

        results.push_back(taskSI);
    }

    return results;
}

void WorkflowManager::editTask(const std::string& agentId, const nikmon::types::EditTask& editTask) {
    auto agent = findAgent(agentId);
    if (agent != nullptr) {
        agent->editTask(editTask);
    }

    _databaseManager->editTask(editTask);
}

void WorkflowManager::removeTask(const std::string& agentId, const std::string& taskId) {
    auto agent = findAgent(agentId);
    if (agent != nullptr) {
        agent->cancelTask(taskId);
    }

    _databaseManager->removeTask(taskId);
}

void WorkflowManager::toggleTask(const std::string& agentId, const std::string& taskId) {
    auto agent = findAgent(agentId);
    if (agent != nullptr) {
        if (agent->cancelTask(taskId)) {
            _databaseManager->toggleTask(taskId, false);
            return;
        }
    }

    auto taskDb = _databaseManager->getTask(taskId);
    if (taskDb == nullptr) {
        throw std::runtime_error("Task with id " + taskId + " was not found");
    }

    auto newState = !taskDb->isActive;

    _databaseManager->toggleTask(taskId, newState);
    if (taskDb->agentId == agent->id && newState) {
        agent->assignTask(*taskDb);
    }
}

nikmon::types::Agent* WorkflowManager::findAgent(const std::string& id) {
    std::lock_guard<std::mutex> lg(_mutex);

    auto existAgentIt = std::find_if(_agents.cbegin(), _agents.cend(), [&id](const auto& agent) {
        return agent->id == id;
    });

    if (existAgentIt == _agents.cend()) {
        return nullptr;
    }

    return existAgentIt->get();
}

void WorkflowManager::addAgent(std::unique_ptr<nikmon::types::Agent> agent) {
    std::lock_guard<std::mutex> lg(_mutex);
    _agents.push_back(std::move(agent));
}
