#pragma once

#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "app/ServiceLocator.h"

class Router : public Poco::Net::HTTPRequestHandlerFactory {
public:
    Router();

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override;

private:
    ServiceLocator* _serviceLocator;
};