#pragma once

#include "types/EditTask.h"

class IAgentsManager {
    using EditTask = nikmon::types::EditTask;
public:
    virtual ~IAgentsManager() = default;

    virtual void assignTask(const std::string& agentId, const EditTask&) = 0;
};