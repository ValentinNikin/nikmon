#include "SQLiteDatabaseManager.h"

#include <Poco/UUIDGenerator.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/File.h>

#include "core/database/repositories/sqlite/SQLiteAgentsRepository.h"
#include "core/database/repositories/sqlite/SQLiteTasksRepository.h"
#include "core/database/repositories/sqlite/SQLiteTasksItemsRepository.h"
#include "core/database/repositories/sqlite/SQLiteTasksItemsErrorsRepository.h"

#include "core/database/types/AgentDB.h"

using namespace Poco::Data::Keywords;
using namespace nikmon::types;

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
                "[Frequency] int NOT NULL, [Delay] int, [Key] nvarchar NOT NULL, [Type] int NOT NULL, [Status] int NOT NULL, "
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

std::vector<std::unique_ptr<AgentDB>> SQLiteDatabaseManager::getAgents() {
    return _agentsRepository->list();
}

std::vector<std::unique_ptr<AgentDB>> SQLiteDatabaseManager::getAgentsByIpAndMachineName(
        const std::string ip, const std::string& machineName) {
    return _agentsRepository->getByIpAndMachineName(ip, machineName);
}

void SQLiteDatabaseManager::createAgent(const EditAgent& editAgent) {
    AgentDB agentDb;
    agentDb.id = Poco::UUIDGenerator().createRandom().toString();
    agentDb.ip = editAgent.ip;
    agentDb.machineName = editAgent.machineName;
    agentDb.heartbeat = editAgent.heartbeat;

    _agentsRepository->insert(agentDb);
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

void SQLiteDatabaseManager::saveTask(const Task& task) {
    TaskDB taskDb;
    taskDb.id = task.id;
    taskDb.agentId = task.agentId;
    taskDb.frequency = task.frequency;
    taskDb.delay = task.delay;
    taskDb.key = task.key;
    taskDb.valueType = task.valueType;

    _tasksRepository->insert(taskDb);
}

std::vector<std::unique_ptr<TaskDB>> SQLiteDatabaseManager::getTasks(const std::string& agentId) {
    return _tasksRepository->list(agentId);
}

std::unique_ptr<TaskDB> SQLiteDatabaseManager::getTask(const std::string& taskId) {
    return _tasksRepository->get(taskId);
}

void SQLiteDatabaseManager::editTask(const EditTask& editTask) {
    auto taskDb = _tasksRepository->get(editTask.id);
    if (taskDb == nullptr) {
        throw std::runtime_error("Task with id " + editTask.id + " not found");
    }

    taskDb->frequency = editTask.frequency;
    taskDb->delay = editTask.delay;
    taskDb->key = editTask.key;
    taskDb->valueType = editTask.valueType;

    _tasksRepository->update(*taskDb);
}

void SQLiteDatabaseManager::toggleTask(const std::string& taskId, const TaskStatus newState) {
    _tasksRepository->toggleState(taskId, newState);
}

void SQLiteDatabaseManager::removeTask(const std::string& taskId) {

}

void SQLiteDatabaseManager::removeTaskResults(const std::string& taskId) {
    _tasksItemsRepositoryUint->remove(taskId);
    _tasksItemsRepositoryFloat->remove(taskId);
    _tasksItemsRepositoryText->remove(taskId);
}

std::vector<std::unique_ptr<TaskResultItem>> SQLiteDatabaseManager::getResults(
        const std::string& taskId, const long from, const long to) {

    auto taskDb = _tasksRepository->get(taskId);
    if (taskDb == nullptr) {
        throw std::runtime_error("Task with id " + taskId + " not found");
    }

    std::vector<std::unique_ptr<TaskResultItem>> results;

    if (taskDb->valueType == TaskValueType::uintType) {
        for (const auto& i : _tasksItemsRepositoryUint->list(taskId, from, to)) {
            results.push_back(std::make_unique<TaskResultItem>(*i));
        }
    }
    else if (taskDb->valueType == TaskValueType::floatType) {
        for (const auto& i : _tasksItemsRepositoryFloat->list(taskId, from, to)) {
            results.push_back(std::make_unique<TaskResultItem>(*i));
        }
    }
    else if (taskDb->valueType == TaskValueType::textType) {
        for (const auto& i : _tasksItemsRepositoryText->list(taskId, from, to)) {
            results.push_back(std::make_unique<TaskResultItem>(*i));
        }
    }

    return results;
}

std::vector<std::unique_ptr<TaskErrorItem>> SQLiteDatabaseManager::getErrors(
        const std::string& taskId, const long from, const long to) {
    std::vector<std::unique_ptr<TaskErrorItem>> errors;

    for (const auto& e : _tasksItemsErrorsRepository->list(taskId, from, to)) {
        errors.push_back(std::make_unique<TaskErrorItem>(*e));
    }

    return errors;
}