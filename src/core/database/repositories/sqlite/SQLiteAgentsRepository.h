#pragma once

#include <Poco/Data/Session.h>

#include "core/database/repositories/IAgentsRepository.h"

class SQLiteAgentsRepository : public IAgentsRepository {
public:
    SQLiteAgentsRepository(const std::string& tableName, const Poco::Data::Session& session);

    std::vector<std::unique_ptr<AgentDB>> list() override;
    bool insert(AgentDB& agent) override;
    std::unique_ptr<AgentDB> get(const std::string& agentId) override;
    bool update(AgentDB& agent) override;
    bool remove(const std::string& agentId) override;

    std::vector<std::unique_ptr<AgentDB>> getByIpAndMachineName(const std::string ip, const std::string& machineName) override;

private:
    std::string _tableName;
    Poco::Data::Session _session;
};
