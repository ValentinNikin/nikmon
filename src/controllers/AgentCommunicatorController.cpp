#include "AgentCommunicatorController.h"

#include <string>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "types/agent-communication/RegistrationRequest.h"
#include "types/agent-communication/RegistrationResponse.h"
#include "types/agent-communication/StatusResponse.h"

using namespace nikmon::types;

AgentCommunicatorController::AgentCommunicatorController(
        const std::shared_ptr<IAgentCommunicator>& agentCommunicator)
        : _agentCommunicator(agentCommunicator),
        _logger(Poco::Logger::get("AgentCommunicatorController")) {
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_POST, "/api/agent/registration", registerAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_POST, "/api/agent/status", statusAgent)
}

void AgentCommunicatorController::registerAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto payload = readPayloadFromRequest(request);

    RegistrationRequest registrationRequest;
    from_json(payload, registrationRequest);

    _logger.debug("'registration' request from %s (%s) agent", registrationRequest.ip, registrationRequest.machineName);

    auto registrationResponse = _agentCommunicator->registerAgent(registrationRequest);
    nlohmann::json responseJson;
    to_json(responseJson, registrationResponse);

    _logger.debug("'registration' request from %s (%s) agent (%s) handled", registrationRequest.ip, registrationRequest.machineName, registrationResponse.id);

    handleHttpStatusCode(200, response);
    std::ostream &outputStream = response.send();
    outputStream << to_string(responseJson);
    outputStream.flush();
}

void AgentCommunicatorController::statusAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto payload = readPayloadFromRequest(request);

    StatusRequest statusRequest;
    from_json(payload, statusRequest);

    _logger.debug("'status' request from % agent", statusRequest.id);

    auto statusResponse = _agentCommunicator->statusAgent(statusRequest);
    nlohmann::json responseJson;
    to_json(responseJson, statusResponse);

    _logger.debug("'status' request from % agent handled", statusRequest.id);

    handleHttpStatusCode(200, response);
    std::ostream &outputStream = response.send();
    outputStream << to_string(responseJson);
    outputStream.flush();
}
