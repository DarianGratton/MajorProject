#pragma once

#include <memory>

#include "NetworkAgent.h"
#include "NetworkParameters.h"

/*
  AgentFactory

  A class that creates new NetworkAgents given a set of parameters.

  Author: Darian G.
*/
class AgentFactory
{
public:

	/* 
	  Default constructor.
	*/
	AgentFactory() = default;

	/*
	  Creates a new NetworkAgent.
	  params:
			- params: Parameters for the network to be created.
	  returns:
			- A new NetworkAgent.
	*/
	std::shared_ptr<NetworkAgent> GetNetworkAgent(NetworkParameters& params);
};
