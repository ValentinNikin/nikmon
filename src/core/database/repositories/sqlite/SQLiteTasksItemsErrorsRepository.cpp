#include "SQLiteTasksItemsErrorsRepository.h"

using namespace Poco::Data::Keywords;

SQLiteTasksItemsErrorsRepository::SQLiteTasksItemsErrorsRepository(
        const std::string& tableName, const Poco::Data::Session& session)
        : _tableName(tableName), _session(session) {}

std::vector<std::unique_ptr<TaskItemErrorDB>> SQLiteTasksItemsErrorsRepository::list(const std::string& taskId) {
    std::vector<std::unique_ptr<TaskItemErrorDB>> tasksItemsErrors;

    TaskItemErrorDB taskItemError;
    std::string taskId_ = taskId;

    Poco::Data::Statement statement =
            (_session << "SELECT TaskId, Time, Message FROM %s WHERE TaskId = ?",
                    into(taskItemError.taskId), into(taskItemError.time), into(taskItemError.message),
                    _tableName, use(taskId_), range(0, 1));

    while (!statement.done() && statement.execute()) {
        tasksItemsErrors.push_back(std::make_unique<TaskItemErrorDB>(taskItemError));
    }

    return tasksItemsErrors;
}

std::vector<std::unique_ptr<TaskItemErrorDB>> SQLiteTasksItemsErrorsRepository::list(const std::string& taskId, long int from, long int to) {
    std::vector<std::unique_ptr<TaskItemErrorDB>> tasksItemsErrors;

    TaskItemErrorDB taskItemError;
    std::string taskId_ = taskId;

    Poco::Data::Statement statement =
            (_session << "SELECT TaskId, Time, Message FROM %s WHERE TaskId = ? AND Time >= ? AND Time <= ?",
                    into(taskItemError.taskId), into(taskItemError.time), into(taskItemError.message),
                    _tableName, use(taskId_), use(from), use(to), range(0, 1));

    while (!statement.done() && statement.execute()) {
        tasksItemsErrors.push_back(std::make_unique<TaskItemErrorDB>(taskItemError));
    }

    return tasksItemsErrors;
}

bool SQLiteTasksItemsErrorsRepository::insert(TaskItemErrorDB& taskItemError) {
    Poco::Data::Statement statement =
            (_session << "INSERT INTO %s VALUES(?, ?, ?)",
                    _tableName, use(taskItemError.taskId), use(taskItemError.time), use(taskItemError.message));
    return statement.execute() != 0;
}

bool SQLiteTasksItemsErrorsRepository::remove(const std::string& taskId) {
    auto taskId_ = taskId;
    Poco::Data::Statement statement =
            (_session << "DELETE FROM %s WHERE TaskId = ?", _tableName, use(taskId_));
    return statement.execute() != 0;
}