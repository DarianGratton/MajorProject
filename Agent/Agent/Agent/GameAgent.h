#pragma once

#include <torch/torch.h>

#include <vector>
#include <memory>

#include "Environment/Environment.h"
#include "Environment/State.h"
#include "Networks/NetworkAgent.h"
#include "Networks/NetworkParameters.h"
#include "Networks/AgentFactory.h"
#include "Visualizer/Visualizer.h"

using namespace std;

/* 
  GameAgent

  Main front-end of the Agent. Defines the type of agent being used and acts as
  a meditator between the user and the internal logic of the agent.

  TODO: Update description once we add the storage system.

  Author: Darian G.
*/
class GameAgent
{
public:

	/* 
	  Constructor for initializing the GameAgent and it's components.
	  params:
			- env: Environment that the agent acts in.
			- params: Network parameters of the Agent to create.
	*/
	GameAgent(shared_ptr<Environment> env, NetworkParameters& params);

	/*
	  Calls the functions neccessary for the agent to predict a set
	  of actions to take given a state.
	  params:
			- state: A state from the environment.
	  returns:
			- A set of actions.
	*/
	vector<float> PredictAction();

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
	inline shared_ptr<Environment> GetEnvironment() { return environment; }

private:

	/* Environment that the agent acts in. */
	shared_ptr<Environment> environment;
	
	/* Agent that acts in an environment. */
	shared_ptr<NetworkAgent> agent;

};