#include "SQLiteDatabaseManager.h"

#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/File.h>

#include "database/repositories/SQLiteAgentsRepository.h"
#include "database/repositories/SQLiteTasksRepository.h"

using namespace Poco::Data::Keywords;

#define AGENTS_TABLE_NAME

SQLiteDatabaseManager::SQLiteDatabaseManager() {
    // TODO: extract parameters from config file
    std::string db = "SQLite";
    std::string path = "nikmon.db";

    std::remove(path.c_str());

    Poco::File file(path);
    auto databaseIsExist = file.exists();

    Poco::Data::SQLite::Connector::registerConnector();
    Poco::Data::Session session(db, path);

    _agentsRepository = std::make_unique<SQLiteAgentsRepository>("Agents", session);
    _tasksRepository = std::make_unique<SQLiteTasksRepository>("Tasks", session);

    if (!databaseIsExist) {
        _agentsRepository->createTable();
        _tasksRepository->createTable();
    }
}

IAgentsRepository* SQLiteDatabaseManager::getAgents() {
    return _agentsRepository.get();
}

ITasksRepository* SQLiteDatabaseManager::getTasks() {
    return _tasksRepository.get();
}