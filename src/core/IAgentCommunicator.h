#pragma once

#include "types/agent-communication/RegistrationRequest.h"
#include "types/agent-communication/RegistrationResponse.h"
#include "types/agent-communication/StatusRequest.h"
#include "types/agent-communication/StatusResponse.h"

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