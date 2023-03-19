#pragma once

#include <stdint.h>

enum class TaskStatus : uint8_t {
    /**
     * Task execution is stopped
     */
    Stopped = 0,

    /**
     * Task is active (agent collect information)
     */
    Active,

    /**
     * Some intermediate state while agent does not confirm the command
     */
    Pending
};