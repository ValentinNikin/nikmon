#pragma once

#include <string>
#include <memory>

#include <nlohmann-json/json.hpp>

#include "types/enums/CommandType.h"
#include "types/enums/TaskFrequency.h"

namespace nikmon {
namespace types {

struct Payload {
    TaskFrequency frequency;
    long int delay;
    std::string key;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Payload, frequency, delay, key);
};

struct Command {
    std::string taskId;
    CommandType type;
    Payload payload;
};

void to_json(nlohmann::json &j, const Command &c);

void from_json(const nlohmann::json &j, Command &c);

}
}