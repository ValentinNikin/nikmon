#include "SQLiteDatabaseManager.h"

#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/File.h>

#include "database/repositories/SQLiteAgentsRepository.h"
#include "database/repositories/SQLiteTasksRepository.h"
#include "database/repositories/SQLiteTasksItemsRepository.h"
#include "database/repositories/SQLiteTasksItemsErrorsRepository.h"

using namespace Poco::Data::Keywords;

#define TABLE_AGENTS "Agents"
#define TABLE_TASKS "Tasks"
#define TABLE_TASKS_ITEMS_uint "TasksItems_uint"
#define TABLE_TASKS_ITEMS_float "TasksItems_float"
#define TABLE_TASKS_ITEMS_text "TasksItems_text"
#define TABLE_TASKS_ITEMS_errors "TasksItems_errors"

#define SQLITE_CONNECTOR "SQLite"

void createDatabaseIfNotExist(const std::string& path) {
    Poco::File file(path);
    if (file.exists()) {
        return;
    }

    Poco::Data::Session session(SQLITE_CONNECTOR, path);

    session << "CREATE TABLE %s ([Id] blob NOT NULL PRIMARY KEY, [MachineName] nvarchar NOT NULL, "
               "[Ip] nvarchar NOT NULL, [Heartbeat] int NOT NULL, [Info] text)", TABLE_AGENTS, now;

    session << "CREATE TABLE %s ([Id] blob NOT NULL PRIMARY KEY, [AgentId] blob NOT NULL, "
                "[Frequency] int NOT NULL, [Delay] int, [Key] nvarchar NOT NULL, [Type] int NOT NULL, "
                "FOREIGN KEY (AgentId) REFERENCES Agents(Id))", TABLE_TASKS, now;

    session << "CREATE TABLE %s ([TaskId] blob NOT NULL, [Time] int NOT NULL, [Value] int, "
               "FOREIGN KEY (TaskId) REFERENCES Tasks(Id))", TABLE_TASKS_ITEMS_uint, now;

    session << "CREATE TABLE %s ([TaskId] blob NOT NULL, [Time] int NOT NULL, [Value] float, "
               "FOREIGN KEY (TaskId) REFERENCES Tasks(Id))", TABLE_TASKS_ITEMS_float, now;

    session << "CREATE TABLE %s ([TaskId] blob NOT NULL, [Time] int NOT NULL, [Value] text, "
               "FOREIGN KEY (TaskId) REFERENCES Tasks(Id))", TABLE_TASKS_ITEMS_text, now;

    session << "CREATE TABLE %s ([TaskId] blob NOT NULL, [Time] int NOT NULL, [Message] text, "
               "FOREIGN KEY (TaskId) REFERENCES Tasks(Id))", TABLE_TASKS_ITEMS_errors, now;
}

SQLiteDatabaseManager::SQLiteDatabaseManager() {
    // TODO: extract parameter from config file
    std::string path = "nikmon.db";
    std::remove(path.c_str());

    Poco::Data::SQLite::Connector::registerConnector();
    createDatabaseIfNotExist(path);

    Poco::Data::Session session(SQLITE_CONNECTOR, path);

    _agentsRepository = std::make_unique<SQLiteAgentsRepository>(TABLE_AGENTS, session);
    _tasksRepository = std::make_unique<SQLiteTasksRepository>(TABLE_TASKS, session);
    _tasksItemsRepositoryUint = std::make_unique<SQLiteTasksItemsRepository<uint>>(TABLE_TASKS_ITEMS_uint, session);
    _tasksItemsRepositoryFloat = std::make_unique<SQLiteTasksItemsRepository<float>>(TABLE_TASKS_ITEMS_float, session);
    _tasksItemsRepositoryText = std::make_unique<SQLiteTasksItemsRepository<std::string>>(TABLE_TASKS_ITEMS_text, session);
    _tasksItemsErrorsRepository = std::make_unique<SQLiteTasksItemsErrorsRepository>(TABLE_TASKS_ITEMS_errors, session);
}

std::vector<std::unique_ptr<AgentDB>> SQLiteDatabaseManager::getAgentsByIpAndMachineName(
        const std::string ip, const std::string& machineName) {
    return _agentsRepository->getByIpAndMachineName(ip, machineName);
}

void SQLiteDatabaseManager::saveTaskItem(const TaskItemDB<uint>& taskItem) {
    _tasksItemsRepositoryUint->insert(const_cast<TaskItemDB<unsigned int>&>(taskItem));
}

void SQLiteDatabaseManager::saveTaskItem(const TaskItemDB<float>& taskItem) {
    _tasksItemsRepositoryFloat->insert(const_cast<TaskItemDB<float>&>(taskItem));
}

void SQLiteDatabaseManager::saveTaskItem(const TaskItemDB<std::string>& taskItem) {
    _tasksItemsRepositoryText->insert(const_cast<TaskItemDB<std::string>&>(taskItem));
}

void SQLiteDatabaseManager::saveTaskItemError(const TaskItemErrorDB& taskItemError) {
    _tasksItemsErrorsRepository->insert(const_cast<TaskItemErrorDB&>(taskItemError));
}