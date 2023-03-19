#pragma once

#include "Controller.h"

#include "core/IAgentsManager.h"

class AgentsController : public Controller {
public:
    AgentsController(const std::shared_ptr<IAgentsManager>& agentsManager);
private:
    void getAgents(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void getAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void createAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void editAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void deleteAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void getTasks(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void assignTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void editTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void removeTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void toggleTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
    std::shared_ptr<IAgentsManager> _agentsManager;
};