#pragma once

#include "types/RegistrationRequest.h"
#include "types/RegistrationResponse.h"
#include "types/StatusRequest.h"
#include "types/StatusResponse.h"

class IAgentCommunicator {
protected:
    using RegistrationResponse = nikmon::types::RegistrationResponse;
    using RegistrationRequest = nikmon::types::RegistrationRequest;
    using StatusResponse = nikmon::types::StatusResponse;
    using StatusRequest = nikmon::types::StatusRequest;
public:
    virtual ~IAgentCommunicator() = default;

    virtual RegistrationResponse registerAgent(const RegistrationRequest&) = 0;
    virtual StatusResponse statusAgent(const StatusRequest&) = 0;
};