#pragma once

#include <torch/torch.h>

#include <vector>

#include "Environment/Environment.h"

using namespace std;

/* 
  GameAgent

  Abstract Class that defines the functionality that each reinforcement learning
  agent has.

  TODO: Return to this as the class expands.

  Author: Darian G.
*/
class GameAgent
{
public:

	/* 
	  Predicts an action given a state.
	  params:
			- state: The state of the environment.
	  returns:
			- A set of actions.
	*/
	virtual vector<float> PredictAction(torch::Tensor state) = 0;
	
	/* 
	  Performs a learning iteration on the agent by performing the calculations
	  to update it's hyper-parameters.
	*/
	virtual void Learn() = 0;
	
	/* 
	  Saves the agent for later use.
	*/
	virtual void SaveModel() = 0;
	
	/* 
	  Loads an previously saved agent.
	*/
	virtual void LoadModel() = 0;

};