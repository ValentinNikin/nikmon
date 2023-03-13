#pragma once

#include <vector>

#include <nlohmann-json/json.hpp>

#include "Command.h"

class StatusResponse {
    int heartbeat;
    std::vector<Command> commands;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StatusResponse, heartbeat, commands);
};