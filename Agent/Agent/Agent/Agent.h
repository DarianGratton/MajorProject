#pragma once

#include <torch/torch.h>

#include <vector>
#include <memory>

#include "Environment/Environment.h"
#include "Environment/State.h"
#include "Networks/NetworkAgent.h"
#include "Networks/NetworkParameters.h"
#include "Networks/AgentFactory.h"
#include "Storage/UtilityStorage.h"

namespace GameAgent
{ 

/* 
  Agent

  Main front-end of the Agent. Defines the type of agent being used and acts as
  a meditator between the user and the internal logic of the agent.

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
	Agent(std::shared_ptr<Environment> env, 
		  GameAgent::Networks::NetworkParameters& params,
		  std::string storageFilename = "AgentStorage.txt");

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
	  Saves the environment's current utility and it's matching initial state to storage.
	*/
	void SaveUtility();

	/* 
	  Searchs storage for states that match the deltas in the inputted state.
	  params:
			- state: State containng deltas to search storage for.
	  returns:
			- A vector containing all the states and utilities that match the inputted deltas.
	*/
	std::vector<std::pair<State, float>> SearchUtilityStorage(State state);

	/*
	  Clears storage of all stored states and utilities.
	*/
	void ClearStorage();

	/*
	  Gets a pointer to the environment for updating.
	*/
	inline std::shared_ptr<Environment> GetEnvironment() { return environment; }

private:

	/* Environment that the agent acts in. */
	std::shared_ptr<Environment> environment;
	
	/* Agent that acts in an environment. */
	std::shared_ptr<NetworkAgent> agent;

	/* Storage system to store the states and average utility. */
	std::unique_ptr<UtilityStorage> storage;

};

} /* namespace GameAgent */