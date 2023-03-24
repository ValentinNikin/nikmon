#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "CommandConfirmation.h"
#include "TaskItem.h"

namespace nikmon {
namespace types {

struct StatusRequest {
    std::string id;
    std::vector<CommandConfirmation> confirmations;
    std::vector<std::unique_ptr<TaskItem>> items;
};

void to_json(nlohmann::json &j, const StatusRequest &r);
void from_json(nlohmann::json &j, StatusRequest &r);

}
}