#pragma once

#include <vector>
#include <memory>

#include "core/database/types/AgentDB.h"

class IAgentsRepository {
public:
    virtual ~IAgentsRepository() = default;

    virtual void createTable() = 0;

    virtual std::vector<std::unique_ptr<AgentDB>> list() = 0;
    virtual bool insert(AgentDB&) = 0;
    virtual std::unique_ptr<AgentDB> get(const std::string& agentId) = 0;
    virtual bool update(AgentDB&) = 0;
    virtual bool remove(const std::string& agentId) = 0;

    virtual std::vector<std::unique_ptr<AgentDB>> getByIpAndMachineName(const std::string ip, const std::string& machineName) = 0;
};