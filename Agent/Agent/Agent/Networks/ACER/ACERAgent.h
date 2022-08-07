#pragma once

#include <torch/torch.h>

#include "ACERParameters.h"
#include "ActorCriticNetwork.h"
#include "ReplayMemory.h"
#include "Trajectory.h"

#include "../NetworkAgent.h"
#include "../../Environment/Environment.h"
#include "../../Environment/State.h"

#include <memory>
#include <vector>

using namespace std;

/*
  ACERAgent

  A class that defines the main front-end of the ACER Algorithm. 
  Includes all the logic that allows for the user to use and update a ACERAgent
  in a environment. 

  Main implementation of the ACER Algorithm is based off an implemention from 
  GitHub user dchetelat using the original paper and a implementation from 
  GitHub user Kaixhin for reference and understanding. While the core of the logic
  is similar, the structure of the program is modified to fit with PyTorch's C++ 
  front-end and to fit more in line with the goals of the project.

  dchetelat's Repo: https://github.com/dchetelat/acer
  Kaixhin's Repo: https://github.com/Kaixhin/ACER
  Orginal Paper: https://arxiv.org/abs/1611.01224v2

  *** Use Case ***

  Author: Darian G.
*/
class ACERAgent : public NetworkAgent
{
public:

	/* 
	  Constructor for initializing the ACERAgent and all it's parameters.
	  params:
			- params: ACERParameters object that contains all the network's parameters.
	*/
	ACERAgent(const ACERParameters& params);

	/*
	  Predicts an action given a state.
	  params:
			- state: The state of the environment.
	  returns:
			- A set of actions.
	*/
	vector<float> PredictAction(torch::Tensor state) override;

	/*
	  Trains the agent by calling the approate function neccessary to update
	  the agent's components and hyper-parameters.
	  params:
			- The environment that the agent is being trained on.
	*/
	void Train(const Environment& environment) override;

	/*
	  Stores input as data in memory.
	  params:
			- state: The current state.
			- action: The action(s) taken.
			- reward: The reward for taking that action.
			- newState: The state resulting from that action.
			- terminal: Whether the new state is terminal or not.
	*/
	void UpdateMemory(
		State state,
		vector<float> actions,
		float reward,
		State newState,
		bool terminal);

	/*
	  Saves the agent for later use.
	*/
	void SaveModel() override;

	/*
	  Loads an previously saved agent.
	*/
	void LoadModel() override;

private:

	/*
	  Performs a learning iteration on the agent by performing the calculations
	  to update it's hyper-parameters. Implementation follows 
	  Algorithm 2 ACER for discrete actions found in the paper.
	  params:
			- trajectories: The trajectories the agent learns from.
			- onPolicy: Whether the agent is learning on-policy or off-policy 
						(one trajectory directly from the environment or multiple from memory).
	*/
	void Learn(vector<Trajectory> trajectories, bool onPolicy);

	/* 
	  Helper function that contains the calculation for Trust Region Optimization used in the
	  ACER algorithm's learning iteration.
	  params:
			- gradients: The gradients to perform optimization on.
			- policy: The orginal policy of the agent given the state.
			- averagePolicy: The average policy of the agent given the state.
	  return: 
			- The new gradients updated with Trust Region Optimizaiton.
	*/
	vector<torch::Tensor> TrustRegion(vector<torch::Tensor> gradients, 
		torch::Tensor policy, torch::Tensor averagePolicy);

	/* Network */
	ActorCriticNetwork actorCritic = nullptr;
	ActorCriticNetwork averageActorCritic = nullptr;
	
	/* Replay Experience Memory */
	unique_ptr<ACERReplayMemory> memory;

	/* Max length of an episode a agent can act in a environment. */
	unsigned int maxEpisodeLength;

	/* Size of the batch sampled from memory. */
	unsigned int batchSize;
	
	/* Length of each Trajectory that is sampled from memory. */
	unsigned int batchTrajectoryLength;

	/* Reward discount factor. */
	float gamma;

	/* Actor loss weight. */
	float biasWeight;

	/* The trucation parameter. */
	int traceMax;

	/* Constraint of the Trust Region Optimization. */
	float trustRegionConstraint;

	/* Decay of the Trust Region Optimization. */
	float trustRegionDecay;

};
