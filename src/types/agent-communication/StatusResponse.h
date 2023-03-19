#pragma once

#include <vector>

#include <nlohmann-json/json.hpp>

#include "Command.h"

namespace nikmon {
namespace types {

struct StatusResponse {
    int heartbeat;
    std::vector<std::unique_ptr<Command>> commands;
};

void to_json(nlohmann::json &j, const StatusResponse &response);

}
}