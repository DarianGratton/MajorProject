#include "AgentFactory.h"

#include <iostream>
#include <typeinfo>

#include "ACER/ACERAgent.h"
#include "ACER/ACERParameters.h"

shared_ptr<NetworkAgent> AgentFactory::GetNetworkAgent(NetworkParameters& params)
{
	if (params.GetNetworkName() == "ACER")
	{
		return make_shared<ACERAgent>(dynamic_cast<ACERParameters&>(params));
	}

	return nullptr;
}
