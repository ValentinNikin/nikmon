#pragma once

#include <Poco/Data/Session.h>

#include "ITasksItemsErrorsRepository.h"

class SQLiteTasksItemsErrorsRepository : public ITasksItemsErrorsRepository {
public:
    SQLiteTasksItemsErrorsRepository(const std::string& tableName, const Poco::Data::Session& session);

    std::vector<std::unique_ptr<TaskItemErrorDB>> list(const std::string& taskId) override;
    std::vector<std::unique_ptr<TaskItemErrorDB>> list(const std::string& taskId, long int from, long int to) override;
    bool insert(TaskItemErrorDB& taskItemError) override;
    bool remove(const std::string& taskId) override;

private:
    std::string _tableName;
    Poco::Data::Session _session;
};