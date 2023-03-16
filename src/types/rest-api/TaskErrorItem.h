#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "core/database/types/TaskItemErrorDB.h"

namespace nikmon {
namespace types {

struct TaskErrorItem {
    std::string taskId;
    std::string message;
    unsigned long time;

    TaskErrorItem(const TaskItemErrorDB& error) {
        taskId = error.taskId;
        message = error.message;
        time = error.time;
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TaskErrorItem, taskId, message, time)

}
}