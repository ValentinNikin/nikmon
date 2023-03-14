#pragma once

#include <vector>
#include <memory>

#include "core/database/types/TaskItemDB.h"

template<typename ValueType>
class ITasksItemsRepository {
public:
    virtual ~ITasksItemsRepository() = default;

    virtual std::vector<std::unique_ptr<TaskItemDB<ValueType>>> list(const std::string& taskId) = 0;
    virtual std::vector<std::unique_ptr<TaskItemDB<ValueType>>> list(const std::string& taskId, long int from, long int to) = 0;
    virtual bool insert(TaskItemDB<ValueType>&) = 0;
    virtual bool remove(const std::string& taskId) = 0;
};