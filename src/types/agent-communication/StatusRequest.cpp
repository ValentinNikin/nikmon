#include "StatusRequest.h"

void nikmon::types::to_json(nlohmann::json &j, const StatusRequest &r) {
    j["id"] = r.id;

    auto confirmationsJsonArr = nlohmann::json::array();
    for (const auto& c : r.confirmations) {
        auto confirmationJson = nlohmann::json();
        to_json(confirmationJson, c);
        confirmationsJsonArr.push_back(confirmationJson);
    }
    j["confirmations"] = confirmationsJsonArr;

    auto itemsJsonArr = nlohmann::json::array();
    for (const auto& i : r.items) {
        auto itemJson = nlohmann::json();
        to_json(itemJson, *i);
        itemsJsonArr.push_back(itemJson);
    }
    j["items"] = itemsJsonArr;
}

void nikmon::types::from_json(nlohmann::json &j, StatusRequest &r) {
    r.id = j.at("id").get<std::string>();

    for (const auto& cJson : j.at("confirmations")) {
        CommandConfirmation cc;
        from_json(cJson, cc);
        r.confirmations.push_back(cc);
    }

    for (const auto& iJson : j.at("items")) {
        auto item = std::make_unique<TaskItem>();
        from_json(iJson, *item);
        r.items.push_back(std::move(item));
    }
}