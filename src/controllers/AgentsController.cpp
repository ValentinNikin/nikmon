#include "AgentsController.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "app/ServiceLocator.h"

#include "utils/Utils.h"

#include "types/rest-api/EditTask.h"
#include "types/rest-api/EditAgent.h"

using namespace nikmon::types;

AgentsController::AgentsController(
        const std::shared_ptr<IAgentsManager>& agentsManager)
        : _agentsManager(agentsManager) {
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/agents", getAgents)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_POST, "/api/agents", createAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_PUT, "/api/agents", editAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/agents/" + GUID_REGEX_EXPRESSION, getAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_DELETE, "/api/agents/" + GUID_REGEX_EXPRESSION, deleteAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/agents/" + GUID_REGEX_EXPRESSION + "/tasks", getTasks)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_POST, "/api/agents/" + GUID_REGEX_EXPRESSION + "/tasks", assignTask)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_PUT, "/api/agents/" + GUID_REGEX_EXPRESSION + "/tasks", editTask)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_PUT, "/api/agents/" + GUID_REGEX_EXPRESSION + "/tasks/" + GUID_REGEX_EXPRESSION + "/toggle", toggleTask)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_DELETE, "/api/agents/" + GUID_REGEX_EXPRESSION + "/tasks/" + GUID_REGEX_EXPRESSION, removeTask)

}

void AgentsController::getAgents(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse& response) {
    auto agents = _agentsManager->getAgents();

    nlohmann::json agentsJson;
    for (const auto& a : agents) {
        nlohmann::json agentJson;
        to_json(agentJson, a);
        agentsJson.push_back(agentJson);
    }

    handleHttpStatusCode(200, response);
    std::ostream &outputStream = response.send();
    outputStream << to_string(agentsJson);
    outputStream.flush();
}

void AgentsController::getAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    // TODO: need to implement this method

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::createAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto payload = readPayloadFromRequest(request);

    EditAgent newAgent;
    from_json(payload, newAgent);

    _agentsManager->createAgent(newAgent);

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::editAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    // TODO: need to implement this method

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::deleteAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    // TODO: need to implement this method

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::getTasks(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto agentId = uriSegments[2];

    auto tasks = _agentsManager->getTasks(agentId);

    nlohmann::json tasksJson;
    for (const auto& t : tasks) {
        nlohmann::json taskJson;
        to_json(tasksJson, t);
        tasksJson.push_back(taskJson);
    }

    handleHttpStatusCode(200, response);
    std::ostream &outputStream = response.send();
    outputStream << to_string(tasksJson);
    outputStream.flush();
}

void AgentsController::assignTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {

    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto agentId = uriSegments[2];

    auto payload = readPayloadFromRequest(request);

    EditTask newTask;
    from_json(payload, newTask);

    _agentsManager->assignTask(agentId, newTask);

    handleHttpStatusCode(200, response);
    std::ostream& outputStream = response.send();
    outputStream.flush();
}

void AgentsController::editTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto agentId = uriSegments[2];

    auto payload = readPayloadFromRequest(request);

    EditTask task;
    from_json(payload, task);

    _agentsManager->editTask(agentId, task);

    handleHttpStatusCode(200, response);
    std::ostream& outputStream = response.send();
    outputStream.flush();
}

void AgentsController::removeTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto agentId = uriSegments[2];
    auto taskId = uriSegments[4];

    _agentsManager->removeTask(agentId, taskId);

    handleHttpStatusCode(200, response);
    std::ostream& outputStream = response.send();
    outputStream.flush();
}

void AgentsController::toggleTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto uriSegments = nikmon::stringUtils::split(request.getURI(), '/');
    auto agentId = uriSegments[2];
    auto taskId = uriSegments[4];

    _agentsManager->toggleTask(agentId, taskId);

    handleHttpStatusCode(200, response);
    std::ostream& outputStream = response.send();
    outputStream.flush();
}