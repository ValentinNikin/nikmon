#pragma once

#include <Poco/Data/Session.h>

#include "core/database/repositories/ITasksRepository.h"

class SQLiteTasksRepository : public ITasksRepository {
public:
    SQLiteTasksRepository(const std::string& tableName, const Poco::Data::Session& session);

    std::vector<std::unique_ptr<TaskDB>> list(const std::string& agentId) override;
    bool insert(TaskDB& task) override;
    std::unique_ptr<TaskDB> get(const std::string& taskId) override;
    bool update(TaskDB& task) override;
    bool toggleState(const std::string& taskId, const TaskStatus newState) override;
    bool remove(const std::string& taskId) override;
private:
    std::string _tableName;
    Poco::Data::Session _session;
};