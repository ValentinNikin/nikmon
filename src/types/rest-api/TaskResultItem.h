#pragma once

#include <string>

#include <nlohmann-json/json.hpp>

#include "core/database/types/TaskItemDB.h"

namespace nikmon {
namespace types {

struct TaskResultItem {
    std::string taskId;
    /// Значение в сериализованном виде
    std::string value;
    unsigned long time;

    TaskResultItem(const TaskItemDB<long>& item) {
        taskId = item.taskId;
        value = std::to_string(item.value);
        time = item.time;
    }

    TaskResultItem(const TaskItemDB<float>& item) {
        taskId = item.taskId;
        value = std::to_string(item.value);
        time = item.time;
    }

    TaskResultItem(const TaskItemDB<std::string>& item) {
        taskId = item.taskId;
        value = item.value;
        time = item.time;
    }


};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TaskResultItem, taskId, value, time)

}
}