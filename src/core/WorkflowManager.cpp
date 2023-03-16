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
    auto existAgents = _databaseManager->getAgentsByIpAndMachineName(registrationRequest.ip, registrationRequest.machineName);
    if (existAgents.empty()) {
        throw std::runtime_error("Not found configuration for the registered agent");
    }

    if (existAgents.size() > 1) {
        // TODO: print something to log, this is wrong situation
    }

    const auto& agentDb = existAgents[0];

    auto agent = findAgent(agentDb->id);
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
        agent = std::make_shared<Agent>(existAgents[0].get());
        std::lock_guard<std::mutex> lg(_agentsListMutex);
        _agents.push_back(agent);
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
        if (ti.status == TaskResultStatus::Error) {
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
    if (agent == nullptr) {
        throw std::runtime_error("Agent is not registered");
    }

    auto task = std::make_shared<Task>(newTask);
    task->agentId = agent->id;

    {
        auto command = std::make_shared<Command>();
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

std::vector<TaskShortInfo> WorkflowManager::getTasks(const std::string& agentId) {
    std::vector<TaskShortInfo> tasks;

    auto agent = findAgent(agentId);
    auto tasksDb = _databaseManager->getTasks(agentId);

    for (const auto& t : tasksDb) {
        TaskShortInfo taskSI;
        taskSI.id = t->id;
        taskSI.key = t->key;
        taskSI.status = t->status;

        if (agent != nullptr) {
            std::lock_guard<std::mutex> lg(agent->tasksMutex);
            auto taskIt = std::find_if(agent->tasks.cbegin(), agent->tasks.cend(), [&t](const auto& task) {
                return task->id == t->id;
            });

            if (taskIt != agent->tasks.cend()) {
                taskSI.status = (*taskIt)->status;
            }
        }

        tasks.push_back(taskSI);
    }

    return tasks;
}

std::shared_ptr<Agent> WorkflowManager::findAgent(const std::string& id) {
    std::lock_guard<std::mutex> lg(_agentsListMutex);
    auto agentIt = std::find_if(_agents.cbegin(), _agents.cend(), [&id](const auto &agent) {
        return agent->id == id;
    });

    if (agentIt == _agents.cend()) {
        return nullptr;
    }

    return *agentIt;
}

void WorkflowManager::editTask(const std::string& agentId, const nikmon::types::EditTask& editTask) {
    _databaseManager->editTask(editTask);

    auto agent = findAgent(agentId);
    if (agent != nullptr) {
        std::shared_ptr<Task> task;
        {
            std::lock_guard<std::mutex> lg(agent->tasksMutex);
            auto taskIt = std::find_if(agent->tasks.cbegin(), agent->tasks.cend(), [&editTask](const auto &task) {
                return task->id == editTask.id;
            });

            if (taskIt != agent->tasks.cend()) {
                task = *taskIt;
            }

        }

        if (task != nullptr) {
            task->acceptChanges(editTask);

            auto command = std::make_shared<Command>();
            command->taskId = task->id;
            command->type = CommandType::Change;
            command->payload.frequency = task->frequency;
            command->payload.delay = task->delay;
            command->payload.key = task->key;

            std::lock_guard<std::mutex> lg(agent->waitableCommandsMutex);
            agent->waitableCommands.push_back(command);
        }
    }
}

void WorkflowManager::toggleTask(const std::string& agentId, const std::string& taskId) {

    auto isTaskActive = false;

    auto agent = findAgent(agentId);
    if (agent != nullptr) {
        std::lock_guard<std::mutex> lg(agent->tasksMutex);
        auto taskIt = std::find_if(agent->tasks.cbegin(), agent->tasks.cend(), [&taskId](const auto& t) {
            return t->id == taskId;
        });

        if (taskIt == agent->tasks.end()) {
            if ((*taskIt)->status == TaskStatus::Pending) {
                // TODO: replace throwing exception to specified instance of exception object
                throw std::runtime_error("Please wait while the previous command is applied to the task");
            }
            else if ((*taskIt)->status == TaskStatus::Stopped) {
                // TODO: this is wrong situation, print something to log
            }

            isTaskActive = true;

            auto command = std::make_shared<Command>();
            command->taskId = taskId;
            command->type = CommandType::Cancel;

            std::lock_guard<std::mutex> innerLg(agent->waitableCommandsMutex);
            agent->waitableCommands.push_back(command);
            _databaseManager->toggleTask(taskId, TaskStatus::Stopped);
            return;
        }
    }

    _databaseManager->toggleTask(taskId, TaskStatus::Active);
    auto taskDb = _databaseManager->getTask(taskId);

    auto task = std::make_shared<Task>(*taskDb);
    task->status = TaskStatus::Pending;
    {
        std::lock_guard<std::mutex> lg(agent->tasksMutex);
        agent->tasks.push_back(task);
    }

    auto command = std::make_shared<Command>();
    command->taskId = task->id;
    command->type = CommandType::Add;
    command->payload.key = task->key;
    command->payload.delay = task->delay;
    command->payload.frequency = task->frequency;
    {
        std::lock_guard<std::mutex> lg(agent->waitableCommandsMutex);
        agent->waitableCommands.push_back(command);
    }
}