#pragma once

#include <string>

#include <Poco/UUIDGenerator.h>

#include "types/enums/TaskFrequency.h"
#include "types/enums/TaskValueType.h"

#include "types/EditTask.h"

namespace nikmon {
namespace types {

struct Task {
    std::string id;
    std::string agentId;
    TaskFrequency frequency;
    int delay;
    std::string key;
    TaskValueType valueType;

    Task(const EditTask& task) {
        id = Poco::UUIDGenerator().createRandom().toString();
        frequency = task.frequency;
        delay = task.delay;
        key = task.key;
        valueType = task.valueType;
    }
};

}
}