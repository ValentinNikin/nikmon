#pragma once

#include <string>

#include "types/enums/TaskFrequency.h"
#include "types/enums/TaskValueType.h"
#include "types/enums/TaskStatus.h"

struct TaskDB {
    std::string id;
    std::string agentId;
    TaskFrequency frequency;
    int delay;
    std::string key;
    TaskValueType valueType;
    TaskStatus status;

    TaskDB() = default;

    TaskDB(const TaskDB& otherTask) {
        id = otherTask.id;
        agentId = otherTask.agentId;
        frequency = otherTask.frequency;
        delay = otherTask.delay;
        key = otherTask.key;
        valueType = otherTask.valueType;
        status = otherTask.status;
    }
};