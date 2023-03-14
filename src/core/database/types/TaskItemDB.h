#pragma once

#include <string>

template<typename ValueType>
struct TaskItemDB {
    std::string taskId;
    ValueType value;
    unsigned long time;

    TaskItemDB<ValueType>() = default;

    TaskItemDB<ValueType>(const TaskItemDB<ValueType>& otherTask) {
        taskId = otherTask.taskId;
        value = otherTask.value;
        time = otherTask.time;
    }
};