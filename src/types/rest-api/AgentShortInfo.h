#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "types/enums/AgentStatus.h"

namespace nikmon {
namespace types {

struct AgentShortInfo {
    std::string id;
    std::string ip;
    std::string machineName;
    AgentStatus status;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AgentShortInfo, id, ip, machineName, status)

}
}