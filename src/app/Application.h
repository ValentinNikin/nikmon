#pragma once

#include <memory>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

class Application : public Poco::Util::ServerApplication {
public:
    Application() = default;

    void setRouter(std::unique_ptr<Poco::Net::HTTPRequestHandlerFactory>);
protected:
    int main(const std::vector<std::string>&) override;
private:
    int _port {8080};
    std::unique_ptr<Poco::Net::HTTPRequestHandlerFactory> _router;
};

