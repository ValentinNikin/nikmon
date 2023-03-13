#pragma once

#include <string>

#include "nlohmann-json/json.hpp"

struct RegistrationResponse {
    std::string id;
    int heartbeat;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RegistrationResponse, id, heartbeat)
};