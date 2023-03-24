#include "Application.h"

#include <iostream>

#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Path.h>
#include <Poco/Util/PropertyFileConfiguration.h>

#include "ServiceLocator.h"
#include "core/ConfigurationManager.h"

namespace {
    Poco::Path getProjectConfigurationsPath() {
        return Poco::Path().home().append(".config/").append("nikmon");
    }

    std::string getConfigPath() {
        return getProjectConfigurationsPath().append("conf").append("server.config").toString();
    }

    std::string getLogConfigPath() {
        return getProjectConfigurationsPath().append("conf").append("logger.properties").toString();
    }
}

Application::Application()
    : _logger(Poco::Logger::get("Application")) {}

void Application::setRouter(std::unique_ptr<Poco::Net::HTTPRequestHandlerFactory> router) {
    _router = std::move(router);
}

int Application::main(const std::vector<std::string>&) {
    if (!init()) {
        return Poco::Util::ServerApplication::EXIT_SOFTWARE;
    }

    auto* httpServerParams = new Poco::Net::HTTPServerParams();

    httpServerParams->setMaxQueued(_httpServerQueueSize);
    httpServerParams->setMaxThreads(_httpServerMaxThreads);

    Poco::Net::HTTPServer httpServer(_router.get(), Poco::Net::ServerSocket(Poco::UInt16(_port)), httpServerParams);

    httpServer.start();
    _logger.information("Application started on the %d port", _port);

    waitForTerminationRequest();
    httpServer.stop();

    return Poco::Util::ServerApplication::EXIT_OK;
}

bool Application::init() {
    try {
        _logger.debug("Reading logger configuration...");
        Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> logConfig(
                new Poco::Util::PropertyFileConfiguration(getLogConfigPath()));
        Poco::Util::LoggingConfigurator logConfigurator;
        logConfigurator.configure(logConfig);
        _logger.information("Logger configuration successfully read");
    } catch (const std::exception& exception) {
        _logger.fatal("Unable to configure logger: %s", exception.what());
        return false;
    }

    try {
        _logger.debug("Reading configuration file...");
        auto configurationManager = ServiceLocator::getInstance()->resolve<ConfigurationManager>();
        configurationManager->loadConfig(getConfigPath());
        _logger.information("Application configuration file successfully read");
    }
    catch (const std::exception& exception) {
        _logger.fatal("Unable to read application configuration file: %s", exception.what());
        return false;
    }

    auto configurationManager = ServiceLocator::getInstance()->resolve<ConfigurationManager>();
    if (configurationManager->hasKey("Port")) {
        _port = configurationManager->getInt("Port");
    }
    else {
        _logger.error("Unable to extract \"Port\" parameter from config");
        return false;
    }

    if (configurationManager->hasKey("HttpServerQueueSize")) {
        _httpServerQueueSize = configurationManager->getInt("HttpServerQueueSize");
    }
    else {
        _logger.error("Unable to extract \"HttpServerQueueSize\" parameter from config");
        return false;
    }

    if (configurationManager->hasKey("HttpServerMaxThreads")) {
        _httpServerMaxThreads = configurationManager->getInt("HttpServerMaxThreads");
    }
    else {
        _logger.error("Unable to extract \"HttpServerMaxThreads\" parameter from config");
        return false;
    }

    return true;
}