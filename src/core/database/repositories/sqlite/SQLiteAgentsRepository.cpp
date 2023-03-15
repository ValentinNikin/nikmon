#include "SQLiteAgentsRepository.h"

using namespace Poco::Data::Keywords;

SQLiteAgentsRepository::SQLiteAgentsRepository(
        const std::string& tableName, const Poco::Data::Session& session)
        : _tableName(tableName), _session(session) {}

std::vector<std::unique_ptr<AgentDB>> SQLiteAgentsRepository::list() {
    std::vector<std::unique_ptr<AgentDB>> agents;

    AgentDB agent;

    Poco::Data::Statement statement =
            (_session << "SELECT Id, MachineName, Ip, Heartbeat, Info FROM %s",
                    into(agent.id), into(agent.machineName), into(agent.ip), into(agent.heartbeat), into(agent.info), _tableName, range(0, 1));

    while (!statement.done() && statement.execute()) {
        agents.push_back(std::make_unique<AgentDB>(agent));
    }

    return agents;
}

bool SQLiteAgentsRepository::insert(AgentDB& agent) {
    Poco::Data::Statement statement =
            (_session << "INSERT INTO %s VALUES(?, ?, ?, ?, ?)",
                    _tableName, use(agent.id), use(agent.machineName), use(agent.ip), use(agent.heartbeat), use(agent.info));
    return statement.execute() != 0;
}

std::unique_ptr<AgentDB> SQLiteAgentsRepository::get(const std::string& agentId) {
    AgentDB agent;

    std::string agentId_ = agentId;

    Poco::Data::Statement statement =
            (_session << "SELECT Id, MachineName, Ip, Heartbeat, Info FROM %s WHERE Id = ?",
                    into(agent.id), into(agent.machineName), into(agent.ip), into(agent.heartbeat), into(agent.info),
                    _tableName, use(agentId_));

    auto affectedRows = statement.execute();
    if (affectedRows == 0) {
        return nullptr;
    }

    return std::make_unique<AgentDB>(agent);
}

bool SQLiteAgentsRepository::update(AgentDB& agent) {
    Poco::Data::Statement statement =
            (_session << "UPDATE %s SET MachineName = ?, Ip = ?, Heartbeat = ?, Info = ? WHERE Id = ?",
                    _tableName, use(agent.machineName), use(agent.ip), use(agent.heartbeat), use(agent.info),
                    use(agent.id));
    return statement.execute() != 0;
}

bool SQLiteAgentsRepository::remove(const std::string& agentId) {
    auto agentId_ = agentId;
    Poco::Data::Statement statement =
            (_session << "DELETE FROM %s WHERE Id = ?", _tableName, use(agentId_));
    return statement.execute() != 0;
}

std::vector<std::unique_ptr<AgentDB>> SQLiteAgentsRepository::getByIpAndMachineName(
        const std::string ip, const std::string& machineName) {
    std::vector<std::unique_ptr<AgentDB>> agents;

    AgentDB agent;

    std::string ip_ = ip;
    std::string machineName_ = machineName;

    Poco::Data::Statement statement =
            (_session << "SELECT Id, MachineName, Ip, Heartbeat, Info FROM %s WHERE MachineName = ? AND Ip = ?",
                    into(agent.id), into(agent.machineName), into(agent.ip), into(agent.heartbeat), into(agent.info), _tableName,
                    use(machineName_), use(ip_), range(0, 1));

    while (!statement.done() && statement.execute()) {
        agents.push_back(std::make_unique<AgentDB>(agent));
    }

    return agents;
}