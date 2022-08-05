#pragma once

#include <memory>

#include "NetworkAgent.h"
#include "NetworkParameters.h"

using namespace std;

class AgentFactory
{
public:

	AgentFactory() = default;

	shared_ptr<NetworkAgent> GetNetworkAgent(NetworkParameters& params);
};
