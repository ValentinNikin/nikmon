#pragma once

#include <string>

#include "nlohmann-json/json.hpp"

struct RegistrationRequest {
    std::string ip;
    std::string machineName;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(RegistrationRequest, ip, machineName)
};
