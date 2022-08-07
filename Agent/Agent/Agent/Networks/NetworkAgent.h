#pragma once

#include <torch/torch.h>

#include <vector>

#include "../Environment/Environment.h"

/*
  NetworkAgent

  Abstract Class that defines the functionality that each reinforcement learning
  agent has.

  Author: Darian G.
*/
class NetworkAgent
{
public:

	/*
	  Predicts an action given a state.
	  params:
			- state: The state of the environment.
	  returns:
			- A set of actions.
	*/
	virtual std::vector<float> PredictAction(torch::Tensor state) = 0;

	/* 
	  Trains the agent by calling the approate function neccessary to update
	  the agent's components and hyper-parameters.
	  params:
			- The environment that the agent is being trained on.
	*/
	virtual void Train(const Environment& environment) = 0;

	/*
	  Saves the agent for later use.
	*/
	virtual void SaveModel() = 0;

	/*
	  Loads an previously saved agent.
	*/
	virtual void LoadModel() = 0;

};