#pragma once

#include "Controller.h"

#include "core/ITasksManager.h"

class TasksController : public Controller {
public:
    TasksController(const std::shared_ptr<ITasksManager>& tasksManager);
private:
    void getTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void removeTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void removeTaskResults(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void getResults(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void getErrors(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
    std::shared_ptr<ITasksManager> _tasksManager;TasksManager::
};