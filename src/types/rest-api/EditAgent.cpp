#include "EditAgent.h"

void nikmon::types::from_json(nlohmann::json& j, EditAgent& agent) {
    agent.ip = j.at("ip").get<std::string>();
    agent.machineName = j.at("machineName").get<std::string>();
    agent.heartbeat = j.at("heartbeat").get<int>();

    if (j.count("id") != 0) {
        agent.id = j.at("id").get<std::string>();
    }
}