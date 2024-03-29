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
#define TABLE_TASKS_ITEMS_long "TasksItems_long"
#define TABLE_TASKS_ITEMS_float "TasksItems_float"
#define TABLE_TASKS_ITEMS_text "TasksItems_text"
#define TABLE_TASKS_ITEMS_errors "TasksItems_errors"

#define SQLITE_CONNECTOR "SQLite"

namespace {
void createDatabaseIfNotExist(const std::string &path) {
    Poco::File file(path);
    if (file.exists()) {
        return;
    }

    Poco::Data::Session session(SQLITE_CONNECTOR, path);

    session << "CREATE TABLE %s ([Id] blob NOT NULL PRIMARY KEY, [MachineName] nvarchar NOT NULL, "
               "[Ip] nvarchar NOT NULL, [Heartbeat] int NOT NULL, [Info] text, UNIQUE(MachineName, Ip))", TABLE_AGENTS, now;

    session << "CREATE TABLE %s ([Id] blob NOT NULL PRIMARY KEY, [AgentId] blob NOT NULL, "
               "[Frequency] int NOT NULL, [Delay] int, [Key] nvarchar NOT NULL, [Type] int NOT NULL, [IsActive] boolean NOT NULL, "
               "FOREIGN KEY (AgentId) REFERENCES Agents(Id))", TABLE_TASKS, now;

    session << "CREATE TABLE %s ([TaskId] blob NOT NULL, [Time] int NOT NULL, [Value] BIGINT, "
               "FOREIGN KEY (TaskId) REFERENCES Tasks(Id))", TABLE_TASKS_ITEMS_long, now;

    session << "CREATE TABLE %s ([TaskId] blob NOT NULL, [Time] int NOT NULL, [Value] float, "
               "FOREIGN KEY (TaskId) REFERENCES Tasks(Id))", TABLE_TASKS_ITEMS_float, now;

    session << "CREATE TABLE %s ([TaskId] blob NOT NULL, [Time] int NOT NULL, [Value] text, "
               "FOREIGN KEY (TaskId) REFERENCES Tasks(Id))", TABLE_TASKS_ITEMS_text, now;

    session << "CREATE TABLE %s ([TaskId] blob NOT NULL, [Time] int NOT NULL, [Message] text, "
               "FOREIGN KEY (TaskId) REFERENCES Tasks(Id))", TABLE_TASKS_ITEMS_errors, now;
}
}

SQLiteDatabaseManager::SQLiteDatabaseManager(const std::shared_ptr<ConfigurationManager>& configurationManager)
    : _logger(Poco::Logger::get("SQLiteDatabaseManager")) {

    std::string databasePath = "./nikmon.db";
    if (configurationManager->hasKey("DatabasePath")) {
        databasePath = configurationManager->getString("DatabasePath");
    }
    else {
        _logger.warning("Unable to extract 'DatabasePath' from config. Default value '%s' will be used", databasePath);
    }

    Poco::Data::SQLite::Connector::registerConnector();
    createDatabaseIfNotExist(databasePath);

    Poco::Data::Session session(SQLITE_CONNECTOR, databasePath);

    _agentsRepository = std::make_unique<SQLiteAgentsRepository>(TABLE_AGENTS, session);
    _tasksRepository = std::make_unique<SQLiteTasksRepository>(TABLE_TASKS, session);
    _tasksItemsRepositoryUint = std::make_unique<SQLiteTasksItemsRepository<long>>(TABLE_TASKS_ITEMS_long, session);
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

std::string SQLiteDatabaseManager::createAgent(const EditAgent& editAgent) {
    AgentDB agentDb;
    agentDb.id = Poco::UUIDGenerator().createRandom().toString();
    agentDb.ip = editAgent.ip;
    agentDb.machineName = editAgent.machineName;
    agentDb.heartbeat = editAgent.heartbeat;

    _agentsRepository->insert(agentDb);

    return agentDb.id;
}

void SQLiteDatabaseManager::saveTaskItems(const std::vector<std::unique_ptr<TaskItem>>& items) {
    for (const auto& item : items) {
        if (item->status == TaskResultStatus::Error) {
            TaskItemErrorDB tieDB;
            tieDB.taskId = item->id;
            tieDB.time = item->time;
            tieDB.message = item->errorMessage;
            _tasksItemsErrorsRepository->insert(tieDB);
            continue;
        }

        auto taskDb = _tasksRepository->get(item->id);
        if (taskDb == nullptr) {
            _logger.warning("Unable to save results from agent. Task with id '%s' not found", item->id);
            continue;
        }

        auto taskValueType = taskDb->valueType;

        if (taskValueType == TaskValueType::longlongType) {
            TaskItemDB<long> tiDB;
            tiDB.taskId = item->id;
            tiDB.time = item->time;
            tiDB.value = atoll(item->value.c_str());
            _tasksItemsRepositoryUint->insert(tiDB);
        }
        else if (taskValueType == TaskValueType::floatType) {
            TaskItemDB<float> tiDB;
            tiDB.taskId = item->id;
            tiDB.time = item->time;
            tiDB.value = atof(item->value.c_str());
            _tasksItemsRepositoryFloat->insert(tiDB);
        }
        else if (taskValueType == TaskValueType::textType) {
            TaskItemDB<std::string> tiDB;
            tiDB.taskId = item->id;
            tiDB.time = item->time;
            tiDB.value = item->value;
            _tasksItemsRepositoryText->insert(tiDB);
        }
        else {
            _logger.error("Unsupported task result type %d", static_cast<int>(taskValueType));
            assert(0 && "Unsupported task result type");
        }
    }
}

std::string SQLiteDatabaseManager::saveTask(const std::string& agentId, const nikmon::types::EditTask& task) {
    auto agentDb = _agentsRepository->get(agentId);
    if (agentDb == nullptr) {
        throw std::runtime_error("Agent with id " + agentId + " not found");
    }

    TaskDB taskDb;
    taskDb.id = Poco::UUIDGenerator().createRandom().toString();
    taskDb.agentId = agentId;
    taskDb.frequency = task.frequency;
    taskDb.delay = task.delay;
    taskDb.key = task.key;
    taskDb.valueType = task.valueType;

    _tasksRepository->insert(taskDb);

    return taskDb.id;
}

std::vector<std::unique_ptr<TaskDB>> SQLiteDatabaseManager::getTasks(const std::string& agentId, const bool onlyActive) {
    return _tasksRepository->list(agentId, onlyActive);
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

void SQLiteDatabaseManager::toggleTask(const std::string& taskId, const bool isActive) {
    _tasksRepository->toggleState(taskId, isActive);
}

void SQLiteDatabaseManager::removeTask(const std::string& taskId) {
    _tasksRepository->remove(taskId);
    removeTaskResults(taskId);
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

    if (taskDb->valueType == TaskValueType::longlongType) {
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