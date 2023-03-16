#include "Controller.h"

#include <regex>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>

bool Controller::isApplicableForRequest(const Poco::Net::HTTPServerRequest& request) {
    std::size_t endpointIndex = SIZE_MAX;
    return tryToFindEndpointForRequest(request, endpointIndex);
}

void Controller::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    std::size_t endpointIndex = SIZE_MAX;
    if (tryToFindEndpointForRequest(request, endpointIndex)) {
        try {
            _endpoints[endpointIndex].handler(request, response);
        }
        catch (const std::exception& exception) {
            handleHttpStatusCode(500, response);
            std::ostream & outputStream = response.send();
            outputStream << toJson(exception);
            outputStream.flush();
        }
    }

    handleHttpStatusCode(501, response);
    std::ostream& errorStream = response.send();
    errorStream.flush();
}

bool Controller::tryToFindEndpointForRequest(const Poco::Net::HTTPServerRequest& request, std::size_t& endPointIndex) {
    endPointIndex = SIZE_MAX;

    auto method = request.getMethod();
    auto uri = request.getURI();

    auto endpoint = std::find_if(_endpoints.cbegin(), _endpoints.cend(), [&method, &uri](const auto& e) {
        if (e.method != method) {
            return false;
        }

        return std::regex_match(uri, e.routeRegex);
    });

    if (endpoint != _endpoints.cend()) {
        endPointIndex = std::distance(_endpoints.cbegin(), endpoint);
        return true;
    }

    return false;
}

void Controller::handleHttpStatusCode(int statusCode, Poco::Net::HTTPServerResponse& response) {
    switch (statusCode) {
        case 200:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
            break;
        case 201:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_CREATED);
            break;
        case 202:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_ACCEPTED);
            break;
        case 204:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NO_CONTENT);
            break;
        case 205:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_RESET_CONTENT);
            break;
        case 206:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT);
            break;
        case 400:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            break;
        case 401:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
            break;
        case 403:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
            break;
        case 404:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
            break;
        case 405:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
            break;
        case 406:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_ACCEPTABLE);
            break;
        case 409:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_CONFLICT);
            break;
        case 410:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_GONE);
            break;
        case 415:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_UNSUPPORTEDMEDIATYPE);
            break;
        case 500:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            break;
        case 501:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_IMPLEMENTED);
            break;
        case 503:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_SERVICE_UNAVAILABLE);
            break;
            // Validating routines throw exceptions all over the program, but are not able to specify
            // an exception code compatible with HTTP. So, the code is left zero. This routine can catch this.
        default:
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    }
}

std::string Controller::toJson(const std::exception& exception) {
    nlohmann::json json;
    json["error"] = exception.what();
    return to_string(json);
}

nlohmann::json Controller::readPayloadFromRequest(Poco::Net::HTTPServerRequest& request) {
    std::string jsonPayload;
    std::istream &inputStream = request.stream();
    Poco::StreamCopier::copyToString(inputStream, jsonPayload);

    return nlohmann::json::parse(jsonPayload);
}