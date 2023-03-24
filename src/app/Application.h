#pragma once

#include <memory>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include <Poco/Logger.h>

class Application : public Poco::Util::ServerApplication {
public:
    Application();

    void setRouter(std::unique_ptr<Poco::Net::HTTPRequestHandlerFactory>);
protected:
    int main(const std::vector<std::string>&) override;
private:
    bool init();

    int _port;
    int _httpServerQueueSize;
    int _httpServerMaxThreads;

    std::unique_ptr<Poco::Net::HTTPRequestHandlerFactory> _router;
    Poco::Logger& _logger;
};

