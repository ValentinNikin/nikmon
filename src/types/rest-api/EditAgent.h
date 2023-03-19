#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

namespace nikmon {
namespace types {

struct EditAgent {
    std::string id;
    std::string ip;
    std::string machineName;
    int heartbeat;
};

void from_json(nlohmann::json &j, EditAgent &response);

}
}