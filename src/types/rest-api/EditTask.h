#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "types/enums/TaskFrequency.h"
#include "types/enums/TaskValueType.h"

namespace nikmon {
namespace types {

struct EditTask {
    std::string id;
    TaskFrequency frequency;
    int delay;
    std::string key;
    TaskValueType valueType;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EditTask, frequency, delay, key, valueType)

}
}