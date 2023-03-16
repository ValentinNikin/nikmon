#include "TasksController.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <nlohmann-json/json.hpp>

#include "utils/Utils.h"

using namespace nikmon::types;

TasksController::TasksController(const std::shared_ptr<ITasksManager>& tasksManager)
        : _tasksManager(tasksManager) {
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/tasks/" + GUID_REGEX_EXPRESSION, getTask)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_DELETE, "/api/tasks/" + GUID_REGEX_EXPRESSION, removeTask)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_DELETE, "/api/tasks/" + GUID_REGEX_EXPRESSION + "/result", removeTaskResults)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/tasks/" + GUID_REGEX_EXPRESSION + "/result?*", getResults)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/tasks/" + GUID_REGEX_EXPRESSION + "/error?*", getResults)
}

void TasksController::getTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {

}

void TasksController::removeTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto taskId = uriSegments[2];

    _tasksManager->removeTask(taskId);

    handleHttpStatusCode(200, response);
    std::ostream& outputStream = response.send();
    outputStream.flush();
}

void TasksController::removeTaskResults(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto taskId = uriSegments[2];

    _tasksManager->removeTaskResults(taskId);

    handleHttpStatusCode(200, response);
    std::ostream& outputStream = response.send();
    outputStream.flush();
}

void TasksController::getResults(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto taskId = uriSegments[2];
    long from = 0;
    long to = 0;

    auto resultItems = _tasksManager->getResults(taskId, from, to);

    nlohmann::json resultsJson;
    for (const auto& i : resultItems) {
        nlohmann::json resultJson;
        to_json(resultJson, *i);
        resultsJson.push_back(resultJson);
    }

    handleHttpStatusCode(200, response);
    std::ostream &outputStream = response.send();
    outputStream << to_string(resultsJson);
    outputStream.flush();
}

void TasksController::getErrors(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto taskId = uriSegments[2];
    long from = 0;
    long to = 0;

    auto errorItems = _tasksManager->getErrors(taskId, from, to);

    nlohmann::json errorsJson;
    for (const auto& i : errorItems) {
        nlohmann::json errorJson;
        to_json(errorJson, *i);
        errorsJson.push_back(errorJson);
    }

    handleHttpStatusCode(200, response);
    std::ostream &outputStream = response.send();
    outputStream << to_string(errorsJson);
    outputStream.flush();
}