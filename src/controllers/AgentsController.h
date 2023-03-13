#pragma once

#include "Controller.h"

class AgentsController : public Controller {
public:
    AgentsController();
private:
    void getAgentsList(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void getAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void createAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void editAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void deleteAgent(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void getAssignedTasks(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void assignTask(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};