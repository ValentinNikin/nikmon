#pragma once

#include <memory>

#include <Poco/Net/HTTPRequestHandler.h>

#include "Controller.h"

class ControllerAdapter : public Poco::Net::HTTPRequestHandler {
public:
    ControllerAdapter(const std::shared_ptr<Controller>& controller);

    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
private:
    std::shared_ptr<Controller> _controller;
};