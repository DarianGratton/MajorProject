#pragma once

#include <torch/torch.h>

#include "ActorCriticNetwork.h"
#include "ReplayMemory.h"
#include "Trajectory.h"

#include <memory>
#include <vector>

/*
  ACERAgent

  TODO: Constructor
  TODO: Destructor
  TODO: UpdateMemory function
  TODO: Learn function
  TODO: Save/Load Model
  TODO: Comments

  Author: Darian G.
*/
class ACERAgent
{
public:
	ACERAgent(unsigned int lr,
		unsigned int nActions, unsigned int nPossibleActions,
		int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims,
		unsigned int memSize, unsigned int batchSize,
		float biasWeight = 0.1f, float gamma = 0.99f, int traceMax = 10);

	~ACERAgent();

	void UpdateMemory(
		State state,
		std::vector<float> actions,
		float reward,
		State newState,
		bool terminal,
		std::vector<float> actionProbabilities);

	/*
	  Follows Algorihtm 2 ACER for discrete actions from the paper.

	  TODO: Take a trejectory rather then a onPolicy boolean, 
	        where trejectory would be struct created from playing the environment or from memory
	*/
	void Learn(std::vector<Trajectory> trajectories, bool onPolicy);

	void SaveModel();
	void LoadModel();

private:
	/* Trust Region. */
	torch::Tensor TrustRegion(std::vector<torch::Tensor> gradients, 
		torch::Tensor policy, torch::Tensor averagePolicy);

	/* Network */
	ActorCriticNetwork actorCritic = nullptr;
	ActorCriticNetwork averageActorCritic = nullptr;

	/* Optimizer */
	torch::optim::Adam* optimizer;
	
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
