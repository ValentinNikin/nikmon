#pragma once

#include <stdint.h>

enum class TaskState : uint8_t {
    /**
     * Task execution is active
     */
    Running = 0,

    /**
     * State when task was assigned to agent, but agent did not confirm the command
     */
    Creating,

    /**
     * State when task configuration changed, but agent did not confirm the command
     */
    Changing,

    /**
     * State when task is cancelled, but agent did not confirm the command
     */
    Cancelling
};