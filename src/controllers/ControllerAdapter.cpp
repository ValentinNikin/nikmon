#include "ControllerAdapter.h"

ControllerAdapter::ControllerAdapter(const std::shared_ptr<Controller>& controller)
    : _controller(controller) {}

void ControllerAdapter::handleRequest(
        Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    return _controller->handleRequest(request, response);
}