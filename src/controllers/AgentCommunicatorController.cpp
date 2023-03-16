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
        : _agentCommunicator(agentCommunicator) {
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_POST, "/api/agent/registration", registerAgent)
    REGISTER_ENDPOINT(Poco::Net::HTTPRequest::HTTP_POST, "/api/agent/status", statusAgent)
}

void AgentCommunicatorController::registerAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto payload = readPayloadFromRequest(request);

    RegistrationRequest registrationRequest;
    from_json(request, registrationRequest);

    auto registrationResponse = _agentCommunicator->registerAgent(registrationRequest);
    nlohmann::json responseJson;
    to_json(responseJson, registrationResponse);

    handleHttpStatusCode(200, response);
    std::ostream &outputStream = response.send();
    outputStream << to_string(responseJson);
    outputStream.flush();
}

void AgentCommunicatorController::statusAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    auto payload = readPayloadFromRequest(request);

    StatusRequest statusRequest;
    from_json(payload, statusRequest);

    auto statusResponse = _agentCommunicator->statusAgent(statusRequest);
    nlohmann::json responseJson;
    to_json(responseJson, statusResponse);

    handleHttpStatusCode(200, response);
    std::ostream &outputStream = response.send();
    outputStream << to_string(responseJson);
    outputStream.flush();
}
