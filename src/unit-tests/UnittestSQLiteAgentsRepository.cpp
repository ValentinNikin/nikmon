#include "gtest/gtest.h"

#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>

#include "core/database/repositories/SQLiteAgentsRepository.h"

TEST(SQLiteAgentsRepository, common) {
    auto dbPath = "test-nikmon.db";

    try {
        Poco::Data::SQLite::Connector::registerConnector();
        Poco::Data::Session session("SQLite", dbPath);

        auto agentsRepository = std::make_unique<SQLiteAgentsRepository>("Agents", session);

        {
            ASSERT_NO_THROW(agentsRepository->createTable());
        }

        {
            auto agents = agentsRepository->list();
            ASSERT_EQ(agents.size(), 0);
        }

        {
            AgentDB agent;
            agent.id = "0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9";
            agent.machineName = "debian";
            agent.ip = "192.168.126.21";
            agent.heartbeat = 5000;
            ASSERT_TRUE(agentsRepository->insert(agent));
        }

        {
            AgentDB agent;
            agent.id = "0f1ca821-2f42-48b3-9c74-1baaa6ebd1c5";
            agent.machineName = "ubuntu";
            agent.ip = "192.168.126.22";
            agent.heartbeat = 3000;
            ASSERT_TRUE(agentsRepository->insert(agent));
        }

        {
            auto agents = agentsRepository->list();
            ASSERT_EQ(agents.size(), 2);

            {
                auto& agent = agents[0];
                ASSERT_EQ(agent->id, "0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9");
                ASSERT_EQ(agent->machineName, "debian");
                ASSERT_EQ(agent->ip, "192.168.126.21");
                ASSERT_EQ(agent->heartbeat, 5000);
                ASSERT_EQ(agent->info, "");
            }

            {
                auto& agent = agents[1];
                ASSERT_EQ(agent->id, "0f1ca821-2f42-48b3-9c74-1baaa6ebd1c5");
                ASSERT_EQ(agent->machineName, "ubuntu");
                ASSERT_EQ(agent->ip, "192.168.126.22");
                ASSERT_EQ(agent->heartbeat, 3000);
                ASSERT_EQ(agent->info, "");
            }
        }

        {
            auto agent = agentsRepository->get("0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9");
            ASSERT_EQ(agent->id, "0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9");
            ASSERT_EQ(agent->machineName, "debian");
            ASSERT_EQ(agent->ip, "192.168.126.21");
            ASSERT_EQ(agent->heartbeat, 5000);
            ASSERT_EQ(agent->info, "");
        }

        {
            {
                AgentDB agent;
                agent.id = "0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9";
                agent.machineName = "debian";
                agent.ip = "192.168.126.30";
                agent.heartbeat = 5000;
                agent.info = "{    \"OS\": {        \"Name\": \"string\",        \"BitDepth\": \"string\"    },    \"CPU\": {        \"Name\": \"string\",        \"Cores\": \"uint\",        \"Instructions\": \"string\"    },    \"Memory\": {        \"Size\": \"uint\"    },    \"Swap\": {        \"Size\": \"uint\"    },    \"Disks\": [        {            \"Name\": \"string\",            \"Size\": \"uint\",            \"MountPoint\": \"string\"        }    ]}";

                ASSERT_TRUE(agentsRepository->update(agent));
            }

            auto agent = agentsRepository->get("0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9");
            ASSERT_EQ(agent->id, "0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9");
            ASSERT_EQ(agent->machineName, "debian");
            ASSERT_EQ(agent->ip, "192.168.126.30");
            ASSERT_EQ(agent->heartbeat, 5000);
            ASSERT_EQ(agent->info, "{    \"OS\": {        \"Name\": \"string\",        \"BitDepth\": \"string\"    },    \"CPU\": {        \"Name\": \"string\",        \"Cores\": \"uint\",        \"Instructions\": \"string\"    },    \"Memory\": {        \"Size\": \"uint\"    },    \"Swap\": {        \"Size\": \"uint\"    },    \"Disks\": [        {            \"Name\": \"string\",            \"Size\": \"uint\",            \"MountPoint\": \"string\"        }    ]}");
        }

        {
            auto agents = agentsRepository->getByIpAndMachineName("192.168.126.30", "debian");
            ASSERT_EQ(agents.size(), 1);
            ASSERT_EQ(agents[0]->id, "0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9");
        }

        {
            ASSERT_TRUE(agentsRepository->remove("0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9"));
            ASSERT_EQ(agentsRepository->list().size(), 2);
        }
    }
    catch (...) {}

    std::remove(dbPath);
}