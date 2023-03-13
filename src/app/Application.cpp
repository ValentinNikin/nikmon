#include "Application.h"

#include <iostream>

#include <Poco/Net/HTTPServer.h>

#include "ServiceLocator.h"

void Application::setRouter(std::unique_ptr<Poco::Net::HTTPRequestHandlerFactory> router) {
    _router = std::move(router);
}

int Application::main(const std::vector<std::string>&) {
    auto* httpServerParams = new Poco::Net::HTTPServerParams();

    // call to make first initialization of ServiceLocator
    ServiceLocator::getInstance();

    // TODO: extract port from config
    // TODO: move param values to config
    httpServerParams->setMaxQueued(100);
    httpServerParams->setMaxThreads(30);

    Poco::Net::HTTPServer httpServer(_router.get(), Poco::Net::ServerSocket(Poco::UInt16(_port)), httpServerParams);

    httpServer.start();
    // TODO: print something to log, about the server is running
    std::cout << "Application started on the " << _port << " port" << std::endl;
    waitForTerminationRequest();
    httpServer.stop();

    return Poco::Util::ServerApplication::EXIT_OK;
}