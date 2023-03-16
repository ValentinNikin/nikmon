#include "StatusResponse.h"

void nikmon::types::to_json(nlohmann::json& j, const StatusResponse& response) {
    j = nlohmann::json();
    j["heartbeat"] = response.heartbeat;
    j["commands"] = nlohmann::json::array();
    for (const auto& c : response.commands) {
        nlohmann::json commandJson;
        to_json(commandJson, *c);
        j["commands"].push_back(commandJson);
    }
}