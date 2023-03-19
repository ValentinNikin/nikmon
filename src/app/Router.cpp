#include "Router.h"

#include <Poco/URI.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "controllers/AgentsController.h"
#include "controllers/AgentCommunicatorController.h"
#include "controllers/TasksController.h"
#include "controllers/ControllerAdapter.h"

#define CREATE_CONTROLLER_IF_APPLICABLE(Controller) \
static auto Controller##Var = _serviceLocator->resolve<Controller>(); \
if (Controller##Var->isApplicableForRequest(request)) {               \
    controller = _serviceLocator->resolve<Controller>();              \
}

Router::Router()
    : _serviceLocator(ServiceLocator::getInstance()) {}

Poco::Net::HTTPRequestHandler* Router::createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
    std::shared_ptr<Controller> controller;

    CREATE_CONTROLLER_IF_APPLICABLE(AgentsController)
    CREATE_CONTROLLER_IF_APPLICABLE(AgentCommunicatorController)
    CREATE_CONTROLLER_IF_APPLICABLE(TasksController)

    if (controller == nullptr) {
        throw std::runtime_error("Unable to process request. Handler not found");
    }

    return new ControllerAdapter(controller);
}