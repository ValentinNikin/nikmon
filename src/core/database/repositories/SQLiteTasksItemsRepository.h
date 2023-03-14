#pragma once

#include <Poco/Data/Session.h>

#include "ITasksItemsRepository.h"

template<typename ValueType>
class SQLiteTasksItemsRepository : public ITasksItemsRepository<ValueType> {
public:
    SQLiteTasksItemsRepository(const std::string& tableName, const Poco::Data::Session& session);

    std::vector<std::unique_ptr<TaskItemDB<ValueType>>> list(const std::string& taskId) override;
    std::vector<std::unique_ptr<TaskItemDB<ValueType>>> list(const std::string& taskId, long int from, long int to) override;
    bool insert(TaskItemDB<ValueType>&) override;
    bool remove(const std::string& taskId) override;

private:
    std::string _tableName;
    Poco::Data::Session _session;
};

template<typename ValueType>
SQLiteTasksItemsRepository<ValueType>::SQLiteTasksItemsRepository(const std::string& tableName, const Poco::Data::Session& session)
        : _tableName(tableName), _session(session) {}

template<typename ValueType>
std::vector<std::unique_ptr<TaskItemDB<ValueType>>> SQLiteTasksItemsRepository<ValueType>::list(const std::string& taskId) {
    using namespace Poco::Data::Keywords;

    std::vector<std::unique_ptr<TaskItemDB<ValueType>>> tasksItems;

    TaskItemDB<ValueType> taskItem;
    std::string taskId_ = taskId;

    Poco::Data::Statement statement =
            (_session << "SELECT TaskId, Time, Value FROM %s WHERE TaskId = ?",
                    into(taskItem.taskId), into(taskItem.time), into(taskItem.value),
                    _tableName, use(taskId_), range(0, 1));

    while (!statement.done() && statement.execute()) {
        tasksItems.push_back(std::make_unique<TaskItemDB<ValueType>>(taskItem));
    }

    return tasksItems;
}

template<typename ValueType>
std::vector<std::unique_ptr<TaskItemDB<ValueType>>> SQLiteTasksItemsRepository<ValueType>::list(
        const std::string& taskId, long int from, long int to) {
    using namespace Poco::Data::Keywords;

    std::vector<std::unique_ptr<TaskItemDB<ValueType>>> tasksItems;

    TaskItemDB<ValueType> taskItem;
    std::string taskId_ = taskId;

    Poco::Data::Statement statement =
            (_session << "SELECT TaskId, Time, Value FROM %s WHERE TaskId = ? AND Time >= ? AND Time <= ?",
                    into(taskItem.taskId), into(taskItem.time), into(taskItem.value),
                    _tableName, use(taskId_), use(from), use(to), range(0, 1));

    while (!statement.done() && statement.execute()) {
        tasksItems.push_back(std::make_unique<TaskItemDB<ValueType>>(taskItem));
    }

    return tasksItems;
}

template<typename ValueType>
bool SQLiteTasksItemsRepository<ValueType>::insert(TaskItemDB<ValueType>& taskItem) {
    using namespace Poco::Data::Keywords;
    Poco::Data::Statement statement =
            (_session << "INSERT INTO %s VALUES(?, ?, ?)",
                    _tableName, use(taskItem.taskId), use(taskItem.time), use(taskItem.value));
    return statement.execute() != 0;
}

template<typename ValueType>
bool SQLiteTasksItemsRepository<ValueType>::remove(const std::string& taskId) {
    using namespace Poco::Data::Keywords;
    auto taskId_ = taskId;
    Poco::Data::Statement statement =
            (_session << "DELETE FROM %s WHERE TaskId = ?", _tableName, use(taskId_));
    return statement.execute() != 0;
}