#pragma once

#include <stdint.h>

enum class CommandType : uint8_t {
    Add = 0,
    Change,
    Cancel
};