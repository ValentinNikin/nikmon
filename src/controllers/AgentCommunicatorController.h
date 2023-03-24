#pragma once

#include <memory>

#include <Poco/Logger.h>

#include "Controller.h"

#include "core/IAgentCommunicator.h"

class AgentCommunicatorController : public Controller {
public:
    AgentCommunicatorController(const std::shared_ptr<IAgentCommunicator>& agentCommunicator);
private:
    void registerAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void statusAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
    std::shared_ptr<IAgentCommunicator> _agentCommunicator;
    Poco::Logger& _logger;
};