#pragma once

#include <vector>
#include <mutex>
#include <list>

#include "core/database/types/AgentDB.h"
#include "core/database/types/TaskDB.h"

#include "Task.h"

#include "types/agent-communication/Command.h"
#include "types/agent-communication/CommandConfirmation.h"
#include "types/agent-communication/StatusResponse.h"

namespace nikmon {
namespace types {

struct Agent {
public:
    std::string id;
    std::string machineName;
    std::string ip;
    int heartbeat;
public:
    Agent(const AgentDB* agent);

    /**
     * Initialize agent with task list
     */
    void init(const std::vector<std::unique_ptr<TaskDB>>& tasks);

    /**
     * Reset task list. Applies when the agent re-registers.
     * For example after unexpected disconnection
     */
    void reset();

    /**
     * Prepares response for the agent "status" request
     */
    StatusResponse prepareStatusResponse(const std::vector<CommandConfirmation>&);

    /**
     * Assign task to agent
     */
    void assignTask(const TaskDB&);

    /**
     * Edit task to agent
     */
    void editTask(const EditTask&);

    /**
     * Cancel task to agent
     */
    bool cancelTask(const std::string& taskId);

    /**
     * Get task by task id
     */
    Task* getTask(const std::string& taskId);

private:
    std::vector<std::unique_ptr<Command>> prepareCommandsList();

    std::recursive_mutex _mutex;
    std::list<std::unique_ptr<Task>> _tasks;
};

}
}

