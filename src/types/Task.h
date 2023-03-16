#pragma once

#include <string>

#include <Poco/UUIDGenerator.h>

#include "types/enums/TaskFrequency.h"
#include "types/enums/TaskValueType.h"
#include "types/enums/TaskStatus.h"

#include "core/database/types/TaskDB.h"

#include "types/rest-api/EditTask.h"

namespace nikmon {
namespace types {

struct Task {
    std::string id;
    std::string agentId;
    TaskFrequency frequency;
    int delay;
    std::string key;
    TaskValueType valueType;
    TaskStatus status;

    Task(const EditTask& task) {
        id = Poco::UUIDGenerator().createRandom().toString();
        frequency = task.frequency;
        delay = task.delay;
        key = task.key;
        valueType = task.valueType;
    }

    Task(const TaskDB& task) {
        id = task.id;
        frequency = task.frequency;
        delay = task.delay;
        key = task.key;
        valueType = task.valueType;
    }

    void acceptChanges(const EditTask& task) {
        frequency = task.frequency;
        delay = task.delay;
        key = task.key;
        valueType = task.valueType;

        status = TaskStatus::Pending;
    }
};

}
}