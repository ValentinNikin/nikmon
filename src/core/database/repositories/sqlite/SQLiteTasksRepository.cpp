#include "SQLiteTasksRepository.h"

using namespace Poco::Data::Keywords;

SQLiteTasksRepository::SQLiteTasksRepository(
        const std::string& tableName, const Poco::Data::Session& session)
        : _tableName(tableName), _session(session) {}

std::vector<std::unique_ptr<TaskDB>> SQLiteTasksRepository::list(const std::string& agentId) {
    std::vector<std::unique_ptr<TaskDB>> tasks;

    TaskDB task;

    std::string agentId_ = agentId;
    int frequency;
    int valueType;
    int taskStatus;

    Poco::Data::Statement statement =
        (_session << "SELECT Id, AgentId, Frequency, Delay, Key, Type, Status FROM %s WHERE AgentId = ?",
                into(task.id), into(task.agentId), into(frequency), into(task.delay),
                into(task.key), into(valueType), into(taskStatus),
                _tableName, use(agentId_));

    while (!statement.done() && statement.execute()) {
        task.frequency = static_cast<TaskFrequency>(frequency);
        task.valueType = static_cast<TaskValueType>(valueType);
        task.status = static_cast<TaskStatus>(taskStatus);
        tasks.push_back(std::make_unique<TaskDB>(task));
    }

    return tasks;
}

bool SQLiteTasksRepository::insert(TaskDB& task) {
    int frequency = static_cast<int>(task.frequency);
    int valueType = static_cast<int>(task.valueType);
    int taskStatus = static_cast<int>(task.status);
    Poco::Data::Statement statement =
            (_session << "INSERT INTO %s VALUES(?, ?, ?, ?, ?, ?, ?)",
                    _tableName,
                    use(task.id), use(task.agentId), use(frequency), use(task.delay),
                    use(task.key), use(valueType), use(taskStatus));
    return statement.execute() != 0;
}

std::unique_ptr<TaskDB> SQLiteTasksRepository::get(const std::string& taskId) {
    TaskDB task;

    std::string taskId_ = taskId;
    int frequency;
    int valueType;
    int taskStatus;

    Poco::Data::Statement statement =
            (_session << "SELECT Id, AgentId, Frequency, Delay, Key, Type, Status FROM %s WHERE Id = ?",
                    into(task.id), into(task.agentId), into(frequency), into(task.delay),
                    into(task.key), into(valueType), into(taskStatus),
                    _tableName, use(taskId_));

    auto affectedRows = statement.execute();
    if (affectedRows == 0) {
        return nullptr;
    }

    task.frequency = static_cast<TaskFrequency>(frequency);
    task.valueType = static_cast<TaskValueType>(valueType);
    task.status = static_cast<TaskStatus>(taskStatus);

    return std::make_unique<TaskDB>(task);
}

bool SQLiteTasksRepository::update(TaskDB& task) {
    int frequency = static_cast<int>(task.frequency);
    int valueType = static_cast<int>(task.valueType);
    int taskStatus = static_cast<int>(task.status);

    Poco::Data::Statement statement =
            (_session << "UPDATE %s SET AgentId = ?, Frequency = ?, Delay = ?, Key = ?, Type = ?, Status = ? WHERE Id = ?",
                    _tableName, use(task.agentId), use(frequency), use(task.delay), use(task.key), use(valueType), use(taskStatus),
                    use(task.id));
    return statement.execute() != 0;
}

bool SQLiteTasksRepository::toggleState(const std::string& taskId, const TaskStatus newState) {
    int taskStatus = static_cast<int>(newState);
    std::string taskId_ = taskId;

    Poco::Data::Statement statement =
            (_session << "UPDATE %s SET Status = ? WHERE Id = ?", _tableName, use(taskStatus), use(taskId_));

    return statement.execute() != 0;
}

bool SQLiteTasksRepository::remove(const std::string& taskId) {
    auto taskId_ = taskId;
    Poco::Data::Statement statement =
            (_session << "DELETE FROM %s WHERE Id = ?", _tableName, use(taskId_));
    return statement.execute() != 0;
}