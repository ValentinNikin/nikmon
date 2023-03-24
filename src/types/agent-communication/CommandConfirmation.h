#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "types/enums/CommandType.h"

namespace nikmon {
namespace types {

struct CommandConfirmation {
    std::string taskId;
    CommandType type;
    bool isConfirmed;
    std::string errorMessage;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CommandConfirmation, taskId, type, isConfirmed, errorMessage)

}
}