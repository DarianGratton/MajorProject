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
		unsigned int memSize, unsigned int batchSize);

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
	void Learn(Trajectory trajectory, bool onPolicy);

	void SaveModel();
	void LoadModel();

private:
	/* Network */
	ActorCriticNetwork actorCritic = nullptr;
	
	/* Replay Experience Memory */
	std::unique_ptr<ACERReplayMemory> memory;

	/* Batch Size (Used for sampling from memory) */
	unsigned int batchSize;
};
