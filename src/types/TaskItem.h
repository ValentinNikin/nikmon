#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "types/enums/TaskStatus.h"

struct TaskItem {
    std::string id;
    TaskStatus status;
    std::string errorMessage;
    std::string value;
    long int time;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TaskItem, id, status, errorMessage, value, time);
};