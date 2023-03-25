#pragma once

#include <string>
#include <vector>
#include <functional>
#include <regex>
#include <memory>
#include <map>

#include <nlohmann-json/json.hpp>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Logger.h>

#define REGISTER_ENDPOINT(method, route, handler) \
_endpoints.emplace_back(method, route, [this](Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) { return handler(request, response); }); \

class Controller : public Poco::Net::HTTPRequestHandler {
public:
    Controller();
    virtual ~Controller() = default;
    /**
     * Determine that controller instance is applicable to handle request
     * @param request request
     * @return true - applicable, false - not applicable
     */
    bool isApplicableForRequest(const Poco::Net::HTTPServerRequest& request);

    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
protected:
    void handleHttpStatusCode(int statusCode, Poco::Net::HTTPServerResponse& response);
    std::string toJson(const std::exception& exception);
    nlohmann::json readPayloadFromRequest(Poco::Net::HTTPServerRequest& request);
    std::map<std::string, std::string> getQueryParameters(const Poco::Net::HTTPServerRequest& request);

    struct EndpointConf {
        std::string method;
        std::regex routeRegex;
        std::function<void(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)> handler;

        EndpointConf() = default;
        EndpointConf(const std::string& method_, const std::string& routeRegex_, std::function<void(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)> handler_)
            : method(method_), routeRegex(routeRegex_), handler(handler_) {}
    };

    const std::string GUID_REGEX_EXPRESSION = "[{]?[0-9a-fA-F]{8}-([0-9a-fA-F]{4}-){3}[0-9a-fA-F]{12}[}]?";

    std::vector<EndpointConf> _endpoints;
private:
    bool tryToFindEndpointForRequest(const Poco::Net::HTTPServerRequest& request, std::size_t& endPointIndex);
    Poco::Logger& _logger;
};