#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "types/enums/TaskStatus.h"

namespace nikmon {
namespace types {

struct TaskItem {
    std::string id;
    TaskStatus status;
    std::string errorMessage;
    std::string value;
    long int time;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TaskItem, id, status, errorMessage, value, time)

}
}