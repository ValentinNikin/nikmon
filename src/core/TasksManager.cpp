#include "TasksManager.h"

using namespace nikmon::types;

TasksManager::TasksManager(const std::shared_ptr<IDatabaseManager>& databaseManager)
        : _databaseManager(databaseManager) {}

std::unique_ptr<TaskInfo> TasksManager::getTask(const std::string& taskId) {
    (void)taskId;
    return nullptr;
}

void TasksManager::removeTask(const std::string& taskId) {
    (void)taskId;
}

void TasksManager::removeTaskResults(const std::string& taskId) {
    _databaseManager->removeTaskResults(taskId);
}

std::vector<std::unique_ptr<TaskResultItem>> TasksManager::getResults(const std::string& taskId, const long from, const long to) {
    return _databaseManager->getResults(taskId, from, to);
}

std::vector<std::unique_ptr<TaskErrorItem>> TasksManager::getErrors(const std::string& taskId, const long from, const long to) {
    return _databaseManager->getErrors(taskId, from, to);
}