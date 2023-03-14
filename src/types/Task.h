#pragma once

#include <string>

#include "types/enums/TaskFrequency.h"
#include "types/enums/TaskValueType.h"

struct Task {
    std::string id;
    TaskFrequency frequency;
    int delay;
    std::string key;
    TaskValueType valueType;
};