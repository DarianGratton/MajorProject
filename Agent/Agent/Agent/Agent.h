#pragma once

#include <torch/torch.h>

#include <vector>
#include <memory>

#include "Environment/Environment.h"
#include "Environment/State.h"
#include "Networks/NetworkAgent.h"
#include "Networks/NetworkParameters.h"
#include "Networks/AgentFactory.h"



/* 
  Agent

  Main front-end of the Agent. Defines the type of agent being used and acts as
  a meditator between the user and the internal logic of the agent.

  TODO: Update description once we add the storage system.

  Author: Darian G.
*/
class Agent
{
public:

	/* 
	  Constructor for initializing the Agent and it's components.
	  params:
			- env: Environment that the agent acts in.
			- params: Network parameters of the Agent to create.
	*/
	Agent(std::shared_ptr<Environment> env, NetworkParameters& params);

	/*
	  Calls the functions neccessary for the agent to predict a set
	  of actions to take given a state.
	  params:
			- state: A state from the environment.
	  returns:
			- A set of actions.
	*/
	std::vector<float> PredictAction();

	/*
	  Calls the functions neccessary to train the agent on the environment.
	*/
	void Train();

	/*
	  Save the agent to a file.
	*/
	void SaveAgent();

	/* 
	  Loads the agent from a file.
	*/
	void LoadAgent();

	/*
	  Gets a pointer to the environment for updating.
	*/
	inline std::shared_ptr<Environment> GetEnvironment() { return environment; }

private:

	/* Environment that the agent acts in. */
	std::shared_ptr<Environment> environment;
	
	/* Agent that acts in an environment. */
	std::shared_ptr<NetworkAgent> agent;

};