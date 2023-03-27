#include "gtest/gtest.h"

#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>

#include "Config.h"
#include "Utils.h"

#include "core/database/repositories/sqlite/SQLiteTasksRepository.h"

TEST(SQLiteTasksRepository, common) {
    auto originalDbPath = Config::getInstance()->getResourcesPath() + "/empty-database.db";
    auto dbPath = TestsUtils::makeCopy(originalDbPath);

    try {
        Poco::Data::SQLite::Connector::registerConnector();
        Poco::Data::Session session("SQLite", dbPath);

        auto tasksRepository = std::make_unique<SQLiteTasksRepository>("Tasks", session);

        {
            auto tasks = tasksRepository->list("0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9", false);
            ASSERT_EQ(tasks.size(), 0);
        }

        {
            TaskDB task;
            task.id = "03299c00-c1ea-11ed-a901-0800200c9a66";
            task.agentId = "0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9";
            task.frequency = TaskFrequency::MultipleTimes;
            task.delay = 1000;
            task.key = "System.Proc.Util";
            task.valueType = TaskValueType::longlongType;
            task.isActive = false;
            ASSERT_TRUE(tasksRepository->insert(task));
        }

        {
            TaskDB task;
            task.id = "454018d0-c1ea-11ed-a901-0800200c9a66";
            task.agentId = "4c9f4dd0-c1ea-11ed-a901-0800200c9a66";
            task.frequency = TaskFrequency::OnceTime;
            task.key = "System.MachineInfo";
            task.valueType = TaskValueType::textType;
            task.isActive = true;
            ASSERT_TRUE(tasksRepository->insert(task));
        }

        {
            auto tasks = tasksRepository->list("0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9", false);
            ASSERT_EQ(tasks.size(), 1);

            {
                auto& task = tasks[0];
                ASSERT_EQ(task->id, "03299c00-c1ea-11ed-a901-0800200c9a66");
                ASSERT_EQ(task->agentId, "0f1ca827-2f4c-48b9-9c73-1baaa6ebd1c9");
                ASSERT_EQ(task->frequency, TaskFrequency::MultipleTimes);
                ASSERT_EQ(task->delay, 1000);
                ASSERT_EQ(task->key, "System.Proc.Util");
                ASSERT_EQ(task->valueType, TaskValueType::longlongType);
                ASSERT_EQ(task->isActive, false);
            }
        }

        {
            auto task = tasksRepository->get("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(task->id, "454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(task->agentId, "4c9f4dd0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(task->frequency, TaskFrequency::OnceTime);
            ASSERT_EQ(task->key, "System.MachineInfo");
            ASSERT_EQ(task->valueType, TaskValueType::textType);
            ASSERT_EQ(task->isActive, true);
        }

        {
            ASSERT_TRUE(tasksRepository->remove("454018d0-c1ea-11ed-a901-0800200c9a66"));
            ASSERT_EQ(tasksRepository->list("4c9f4dd0-c1ea-11ed-a901-0800200c9a66", false).size(), 0);
        }
    }
    catch (const std::exception& exception) {
        std::cout << "Exception occurred: " << exception.what() << std::endl;
        ASSERT_TRUE(false);
    }

    std::remove(dbPath.c_str());
}