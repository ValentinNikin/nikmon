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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EditAgent, id, ip, machineName, heartbeat)

}
}