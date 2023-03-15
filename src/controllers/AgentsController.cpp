#include "AgentsController.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "app/ServiceLocator.h"

#include "utils/Utils.h"

#include "types/EditTask.h"

using namespace nikmon::types;

AgentsController::AgentsController(
        const std::shared_ptr<IAgentsManager>& agentsManager)
        : _agentsManager(agentsManager) {
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/agents", getAgentsList)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_POST, "/api/agents", createAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_PUT, "/api/agents", editAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/agents/" + GUID_REGEX_EXPRESSION, getAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_DELETE, "/api/agents/" + GUID_REGEX_EXPRESSION, deleteAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_GET, "/api/agents/" + GUID_REGEX_EXPRESSION + "/tasks", getAssignedTasks)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_POST, "/api/agents/" + GUID_REGEX_EXPRESSION + "/tasks", assignTask)
}

void AgentsController::getAgentsList(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::getAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::createAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::editAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::deleteAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::getAssignedTasks(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    (void)request;

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

void AgentsController::assignTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {

    auto uriSplitted = nikmon::stringUtils::split(request.getURI(), '/');
    auto agentId = uriSplitted[2];

    auto payload = readPayloadFromRequest(request);

    EditTask editTask;
    from_json(payload, editTask);

    _agentsManager->assignTask(agentId, editTask);

    handleHttpStatusCode(200, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}