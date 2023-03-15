#include "WorkflowManager.h"

#include <algorithm>

#include "types/Task.h"
#include "types/enums/TaskStatus.h"
#include "types/enums/CommandType.h"

using Task = nikmon::types::Task;

WorkflowManager::WorkflowManager(
        const std::shared_ptr<IDatabaseManager>& databaseManager)
        : _databaseManager(databaseManager) {}

nikmon::types::RegistrationResponse WorkflowManager::registerAgent(const RegistrationRequest& registrationRequest) {
    std::shared_ptr<Agent> agent;
    {
        std::lock_guard<std::mutex> lg(_agentsListMutex);
        auto activeAgentIt = std::find_if(_agents.cbegin(), _agents.cend(), [&registrationRequest](const auto &agent) {
            return agent->machineName == registrationRequest.machineName && agent->ip == registrationRequest.ip;
        });

        if (activeAgentIt != _agents.cend()) {
            agent = *activeAgentIt;
        }
    }

    if (agent != nullptr) {
        // clear agent state
        {
            std::lock_guard<std::mutex> lg(agent->tasksMutex);
            agent->tasks.clear();
        }
        {
            std::lock_guard<std::mutex> lg(agent->waitableCommandsMutex);
            agent->waitableCommands.clear();
        }
    }
    else {
        // extract agent configuration from database and add to collection
        auto existAgents = _databaseManager->getAgentsByIpAndMachineName(
                registrationRequest.ip, registrationRequest.machineName);
        if (existAgents.empty()) {
            throw std::runtime_error("Not found configuration for the registered agent");
        }

        if (existAgents.size() > 1) {
            // TODO: print something to log, this is wrong situation
        }

        agent = std::make_shared<Agent>(existAgents[0].get());
        std::lock_guard<std::mutex> lg(_agentsListMutex);
        _agents.push_back(agent);
    }

    RegistrationResponse registrationResponse;

    registrationResponse.id = agent->id;
    registrationResponse.heartbeat = agent->heartbeat;

    return registrationResponse;
}

nikmon::types::StatusResponse WorkflowManager::statusAgent(const StatusRequest& statusRequest) {
    std::shared_ptr<Agent> agent;
    {
        std::lock_guard<std::mutex> lg(_agentsListMutex);
        auto agentIt = std::find_if(_agents.cbegin(), _agents.cend(), [id = statusRequest.id](const auto &agent) {
            return agent->id == id;
        });

        if (agentIt == _agents.cend()) {
            throw std::runtime_error("Agent is not registered");
        }

        agent = *agentIt;
    }

    // process commands confirmations
    {
        std::lock_guard<std::mutex> lg(agent->waitableCommandsMutex);
        for (const auto &c: statusRequest.confirmations) {
            auto agentWaitableCommandIt = std::find_if(
                    agent->waitableCommands.cbegin(), agent->waitableCommands.cend(),
                    [&c](const auto &wc) {
                        return wc->taskId == c.taskId && wc->type == c.type;
                    });

            if (agentWaitableCommandIt == agent->waitableCommands.cend()) {
                // TODO: print something to log
                continue;
            }

            agent->waitableCommands.erase(agentWaitableCommandIt);
        }
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

        TaskValueType taskValueType;
        {
            std::lock_guard<std::mutex> lg(agent->tasksMutex);
            auto taskIt = std::find_if(agent->tasks.cbegin(), agent->tasks.cend(),
                                       [&ti](const auto &task) {
                                           return task->id == ti.id;
                                       });

            if (taskIt == agent->tasks.cend()) {
                // TODO: print something to log
                continue;
            }

            taskValueType = (*taskIt)->valueType;
        }

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

    StatusResponse statusResponse;

    statusResponse.heartbeat = agent->heartbeat;
    {
        std::lock_guard<std::mutex> lg(agent->waitableCommandsMutex);
        statusResponse.commands = agent->waitableCommands;
    }

    return statusResponse;
}

void WorkflowManager::assignTask(const std::string& agentId, const EditTask& newTask) {
    std::shared_ptr<Agent> agent;
    {
        std::lock_guard<std::mutex> lg(_agentsListMutex);
        auto agentIt = std::find_if(_agents.cbegin(), _agents.cend(), [&agentId](const auto &agent) {
            return agent->id == agentId;
        });

        if (agentIt == _agents.cend()) {
            throw std::runtime_error("Agent is not registered");
        }

        agent = *agentIt;
    }

    auto task = std::make_shared<Task>(newTask);
    task->agentId = agent->id;

    {
        auto command = std::make_shared<nikmon::types::Command>();
        command->taskId = task->id;
        command->type = CommandType::Add;
        command->payload.key = task->key;
        command->payload.delay = task->delay;
        command->payload.frequency = task->frequency;

        std::lock_guard<std::mutex> lg(agent->waitableCommandsMutex);
        agent->waitableCommands.push_back(command);
    }

    {
        std::lock_guard<std::mutex> lg(agent->tasksMutex);
        agent->tasks.push_back(task);
    }

    _databaseManager->saveTask(*task);
}