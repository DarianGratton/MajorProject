#pragma once

#include <torch/torch.h>

#include "ActorCriticNetwork.h"
#include "ReplayMemory.h"
#include "Trajectory.h"

#include "../../Environment/State.h"
/* Delete this and replace with Environment class. */
#include "../TestEnvironment/TestEnvironment.h"

#include <memory>
#include <vector>

using namespace RLGameAgent;

/*
  ACERAgent

  TODO: Constructor: most variables should have default values
  TODO: Learn function
  TODO: Save/Load Model
  TODO: Comments
  TODO: Fix mess with nActions and nPossibleActions

  Author: Darian G.
*/
class ACERAgent
{
public:
	ACERAgent(float lr,
		unsigned int nActions, unsigned int nPossibleActions,
		int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims,
		unsigned int memSize = 10000, unsigned int maxEpisodeLength = 256, unsigned int batchSize = 16,
		float biasWeight = 0.1f, float gamma = 0.99f, int traceMax = 10);

	~ACERAgent();

	/* TODO: Not going to be void in the future. */
	void Run(TestEnvironment& env, unsigned int nEpisodes, unsigned int episodeLength);

	std::pair<int, std::vector<float>> PredictAction(State observation);

	void UpdateMemory(
		State state,
		std::vector<int> actions,
		float reward,
		State newState,
		bool terminal,
		std::vector<float> actionProbabilities);

	void SaveModel();
	void LoadModel();

	// TODO: From here down needs to be private, set to public just for test environment
	/*
	  Follows Algorihtm 2 ACER for discrete actions from the paper.

	  TODO: Take a trejectory rather then a onPolicy boolean,
			where trejectory would be struct created from playing the environment or from memory
	*/
	void Learn(std::vector<Trajectory> trajectories);

	void OnPolicyLearn(std::vector<Trajectory> trajectory);

	/* Trust Region. */
	std::vector<torch::Tensor> TrustRegion(std::vector<torch::Tensor> gradients, 
		torch::Tensor policy, torch::Tensor averagePolicy);

	/* Network */
	ActorCriticNetwork actorCritic = nullptr;
	ActorCriticNetwork averageActorCritic = nullptr;

	/* Optimizer */
	// torch::optim::Adam* optimizer;
	
	/* Replay Experience Memory */
	std::unique_ptr<ACERReplayMemory> memory;

	/* Batch Size (Used for sampling from memory) */
	unsigned int batchSize;

	/* Discount factor. */
	float gamma;

	/* Actor loss weight*/
	float biasWeight;

	/* ??? */
	int traceMax;
};
