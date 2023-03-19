#include "EditTask.h"

void nikmon::types::from_json(nlohmann::json &j, EditTask &task) {
    task.frequency = static_cast<TaskFrequency>(j.at("frequency").get<int>());
    task.delay = j.at("delay").get<int>();
    task.key = j.at("key").get<std::string>();
    task.valueType = static_cast<TaskValueType>(j.at("valueType").get<int>());

    if (j.count("id") != 0) {
        task.id = j.at("id").get<std::string>();
    }
}
