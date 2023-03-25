#include "gtest/gtest.h"

#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>

#include "Config.h"
#include "Utils.h"

#include "core/database/repositories/sqlite/SQLiteTasksItemsRepository.h"

TEST(SQLiteTasksItemsRepository, uint) {
    auto originalDbPath = Config::getInstance()->getResourcesPath() + "/empty-database.db";
    auto dbPath = TestsUtils::makeCopy(originalDbPath);

    try {
        Poco::Data::SQLite::Connector::registerConnector();
        Poco::Data::Session session("SQLite", dbPath);

        auto tasksItemsRepository = std::make_unique<SQLiteTasksItemsRepository<uint>>("TasksItems_long", session);

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 0);
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66", 10, 20);
            ASSERT_EQ(items.size(), 0);
        }

        {
            TaskItemDB<uint> taskItem;
            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 5;
            taskItem.value = 70;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 7;
            taskItem.value = 71;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 12;
            taskItem.value = 89;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 15;
            taskItem.value = 89;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 21;
            taskItem.value = 70;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 5);
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66", 10, 20);
            ASSERT_EQ(items.size(), 2);
        }

        {
            ASSERT_TRUE(tasksItemsRepository->remove("454018d0-c1ea-11ed-a901-0800200c9a66"));
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 0);
        }
    }
    catch (const std::exception& exception) {
        std::cout << "Exception occurred: " << exception.what() << std::endl;
        ASSERT_TRUE(false);
    }

    std::remove(dbPath.c_str());
}

TEST(SQLiteTasksItemsRepository, float) {
    auto originalDbPath = Config::getInstance()->getResourcesPath() + "/empty-database.db";
    auto dbPath = TestsUtils::makeCopy(originalDbPath);

    try {
        Poco::Data::SQLite::Connector::registerConnector();
        Poco::Data::Session session("SQLite", dbPath);

        auto tasksItemsRepository = std::make_unique<SQLiteTasksItemsRepository<float>>("TasksItems_float", session);

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 0);
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66", 10, 20);
            ASSERT_EQ(items.size(), 0);
        }

        {
            TaskItemDB<float> taskItem;
            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 5;
            taskItem.value = 70.1;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 7;
            taskItem.value = 71.1;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 12;
            taskItem.value = 89.1;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 15;
            taskItem.value = 89.1;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 21;
            taskItem.value = 70.1;
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 5);
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66", 10, 20);
            ASSERT_EQ(items.size(), 2);
        }

        {
            ASSERT_TRUE(tasksItemsRepository->remove("454018d0-c1ea-11ed-a901-0800200c9a66"));
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 0);
        }
    }
    catch (const std::exception& exception) {
        std::cout << "Exception occurred: " << exception.what() << std::endl;
        ASSERT_TRUE(false);
    }

    std::remove(dbPath.c_str());
}

TEST(SQLiteTasksItemsRepository, text) {
    auto originalDbPath = Config::getInstance()->getResourcesPath() + "/empty-database.db";
    auto dbPath = TestsUtils::makeCopy(originalDbPath);

    try {
        Poco::Data::SQLite::Connector::registerConnector();
        Poco::Data::Session session("SQLite", dbPath);

        auto tasksItemsRepository = std::make_unique<SQLiteTasksItemsRepository<std::string>>("TasksItems_float", session);

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 0);
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66", 10, 20);
            ASSERT_EQ(items.size(), 0);
        }

        {
            TaskItemDB<std::string> taskItem;
            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 5;
            taskItem.value = "70.1";
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 7;
            taskItem.value = "71.1";
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 12;
            taskItem.value = "89.1";
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 15;
            taskItem.value = "89.1";
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));

            taskItem.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItem.time = 21;
            taskItem.value = "70.1";
            ASSERT_TRUE(tasksItemsRepository->insert(taskItem));
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 5);
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66", 10, 20);
            ASSERT_EQ(items.size(), 2);
        }

        {
            ASSERT_TRUE(tasksItemsRepository->remove("454018d0-c1ea-11ed-a901-0800200c9a66"));
        }

        {
            auto items = tasksItemsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 0);
        }
    }
    catch (const std::exception& exception) {
        std::cout << "Exception occurred: " << exception.what() << std::endl;
        ASSERT_TRUE(false);
    }

    std::remove(dbPath.c_str());
}