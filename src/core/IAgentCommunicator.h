#pragma once

#include "types/RegistrationRequest.h"
#include "types/RegistrationResponse.h"
#include "types/StatusRequest.h"
#include "types/StatusResponse.h"

class IAgentCommunicator {
public:
    virtual ~IAgentCommunicator() = default;

    virtual RegistrationResponse registerAgent(const RegistrationRequest&) = 0;
    virtual StatusResponse statusAgent(const StatusRequest&) = 0;
};