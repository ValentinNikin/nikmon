#pragma once

#include <string>

#include "nlohmann-json/json.hpp"

namespace nikmon {
namespace types {

struct RegistrationRequest {
    std::string ip;
    std::string machineName;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RegistrationRequest, ip, machineName)

}
}
