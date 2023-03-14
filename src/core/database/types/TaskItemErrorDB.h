#pragma once

#include <string>

struct TaskItemErrorDB {
    std::string taskId;
    std::string message;
    unsigned long time;

    TaskItemErrorDB() = default;

    TaskItemErrorDB(const TaskItemErrorDB& other) {
        taskId = other.taskId;
        message = other.message;
        time = other.time;
    }
};
