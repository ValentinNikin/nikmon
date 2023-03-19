#include "Agent.h"

using namespace nikmon::types;

Agent::Agent(const AgentDB* agent)
    : id(agent->id),
      machineName(agent->machineName),
      ip(agent->ip),
      heartbeat(agent->heartbeat) {}

void Agent::init(const std::vector<std::unique_ptr<TaskDB>>& tasks) {
    std::lock_guard<std::recursive_mutex> lg(_mutex);
    for (const auto& t : tasks) {
        assignTask(*t);
    }
}

void Agent::reset() {
    std::lock_guard<std::recursive_mutex> lg(_mutex);
    _tasks.clear();
}

StatusResponse Agent::prepareStatusResponse(const std::vector<CommandConfirmation>& confirmations) {
    std::lock_guard<std::recursive_mutex> lg(_mutex);

    for (const auto& c: confirmations) {
        auto existTaskIt = std::find_if(_tasks.cbegin(), _tasks.cend(), [&c](const auto& task) { return task->getId() == c.taskId; });
        if (existTaskIt == _tasks.cend()) {
            // TODO: print something to log
            continue;
        }

        if (c.type == CommandType::Cancel) {
            _tasks.erase(existTaskIt);
        }
        else {
            existTaskIt->get()->acceptConfirmation(c);
        }
    }

    StatusResponse statusResponse;

    statusResponse.heartbeat = heartbeat;
    statusResponse.commands = prepareCommandsList();

    return statusResponse;
}

std::vector<std::unique_ptr<Command>> Agent::prepareCommandsList() {
    std::vector<std::unique_ptr<Command>> commands;

    for (const auto& t : _tasks) {
        auto command = t->constructCommand();
        if (command != nullptr) {
            commands.push_back(std::move(command));
        }
    }

    return commands;
}

void Agent::assignTask(const TaskDB& newTask) {
    const std::string& taskId = newTask.id;
    std::lock_guard<std::recursive_mutex> lg(_mutex);

    if (std::any_of(_tasks.cbegin(), _tasks.cend(), [&taskId](const auto& task) { return task->getId() == taskId; })) {
        throw std::runtime_error("Task with id " + taskId + " already exist");
    }

    _tasks.push_back(std::make_unique<Task>(newTask));
}

void Agent::editTask(const EditTask& task) {
    const std::string& taskId = task.id;
    std::lock_guard<std::recursive_mutex> lg(_mutex);

    auto existTaskIt = std::find_if(_tasks.cbegin(), _tasks.cend(), [&taskId](const auto& task) { return task->getId() == taskId; });
    if (existTaskIt == _tasks.cend()) {
        throw std::runtime_error("Task with id " + taskId + " not found");
    }

    existTaskIt->get()->acceptChanges(task);
}

bool Agent::cancelTask(const std::string& taskId) {
    std::lock_guard<std::recursive_mutex> lg(_mutex);

    auto existTaskIt = std::find_if(_tasks.cbegin(), _tasks.cend(), [&taskId](const auto& task) { return task->getId() == taskId; });
    if (existTaskIt == _tasks.cend()) {
        return false;
    }

    existTaskIt->get()->acceptCancel();

    return true;
}

Task* Agent::getTask(const std::string& taskId) {
    std::lock_guard<std::recursive_mutex> lg(_mutex);

    auto existTaskIt = std::find_if(_tasks.cbegin(), _tasks.cend(), [&taskId](const auto& task) { return task->getId() == taskId; });
    if (existTaskIt == _tasks.cend()) {
        return nullptr;
    }

    return existTaskIt->get();
}