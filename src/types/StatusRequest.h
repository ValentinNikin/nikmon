#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "CommandConfirmation.h"
#include "TaskItem.h"

struct StatusRequest {
    std::string id;
    std::vector<CommandConfirmation> confirmations;
    std::vector<TaskItem> items;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StatusRequest, id, confirmations, items);
};