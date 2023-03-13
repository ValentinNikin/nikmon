#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "types/enums/CommandType.h"

struct CommandConfirmation {
    std::string taskId;
    CommandType type;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CommandConfirmation, taskId, type)
};