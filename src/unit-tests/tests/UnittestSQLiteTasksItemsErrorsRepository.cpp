#include "gtest/gtest.h"

#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>

#include "Config.h"
#include "Utils.h"

#include "core/database/repositories/sqlite/SQLiteTasksItemsErrorsRepository.h"

TEST(SQLiteTasksItemsErrorsRepository, common) {
    auto originalDbPath = Config::getInstance()->getResourcesPath() + "/empty-database.db";
    auto dbPath = TestsUtils::makeCopy(originalDbPath);

    try {
        Poco::Data::SQLite::Connector::registerConnector();
        Poco::Data::Session session("SQLite", dbPath);

        auto tasksItemsErrorsRepository = std::make_unique<SQLiteTasksItemsErrorsRepository>("TasksItems_errors", session);

        {
            auto items = tasksItemsErrorsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 0);
        }

        {
            auto items = tasksItemsErrorsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66", 10, 20);
            ASSERT_EQ(items.size(), 0);
        }

        {
            TaskItemErrorDB taskItemError;
            taskItemError.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItemError.time = 5;
            taskItemError.message = "1";
            ASSERT_TRUE(tasksItemsErrorsRepository->insert(taskItemError));

            taskItemError.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItemError.time = 7;
            taskItemError.message = "2";
            ASSERT_TRUE(tasksItemsErrorsRepository->insert(taskItemError));

            taskItemError.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItemError.time = 12;
            taskItemError.message = "3";
            ASSERT_TRUE(tasksItemsErrorsRepository->insert(taskItemError));

            taskItemError.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItemError.time = 15;
            taskItemError.message = "4";
            ASSERT_TRUE(tasksItemsErrorsRepository->insert(taskItemError));

            taskItemError.taskId = "454018d0-c1ea-11ed-a901-0800200c9a66";
            taskItemError.time = 21;
            taskItemError.message = "5";
            ASSERT_TRUE(tasksItemsErrorsRepository->insert(taskItemError));
        }

        {
            auto items = tasksItemsErrorsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 5);
        }

        {
            auto items = tasksItemsErrorsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66", 10, 20);
            ASSERT_EQ(items.size(), 2);
        }

        {
            ASSERT_TRUE(tasksItemsErrorsRepository->remove("454018d0-c1ea-11ed-a901-0800200c9a66"));
        }

        {
            auto items = tasksItemsErrorsRepository->list("454018d0-c1ea-11ed-a901-0800200c9a66");
            ASSERT_EQ(items.size(), 0);
        }
    }
    catch (const std::exception& exception) {
        std::cout << "Exception occurred: " << exception.what() << std::endl;
        ASSERT_TRUE(false);
    }

    std::remove(dbPath.c_str());
}