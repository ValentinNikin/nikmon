#pragma once

#include <stdint.h>

enum class TaskAction : uint8_t {
    start = 0,
    stop,
    clearHistory,
    remove
};