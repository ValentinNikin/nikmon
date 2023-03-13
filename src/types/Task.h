#pragma once

#include <string>

#include "types/enums/TaskFrequency.h"

struct Task {
    int id;
    TaskFrequency frequency;
    int delay;
    std::string key;
};