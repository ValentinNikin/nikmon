#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "types/enums/TaskStatus.h"

namespace nikmon {
namespace types {

struct TaskShortInfo {
    std::string id;
    TaskStatus status;
    std::string key;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TaskShortInfo, id, status, key)

}
}