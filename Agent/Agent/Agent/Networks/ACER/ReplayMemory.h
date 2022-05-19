#pragma once

#include <torch/torch.h>

#include <deque>

#include "Trajectory.h"
#include "../../Environment/Reward.h"
#include "../../Environment/State.h"

using namespace std;
using namespace RLGameAgent;

/*
  ReplayMemory

  A class that defines the memory of the ACER Algorithm.
  Stores the state, action, reward, nextState, and whether it was a
  terminal state for each action taken in the environment.

  Note: State size and number of actions per state must remain the same as when initialized.

  TODO: Modify SampleMemory to sample multiple trajectories and batch them together.
  TODO: Testing of Save/Load memCounter.
  TODO: Save/Load into created directory.

  Author: Darian G.
*/
class ACERReplayMemory
{
public:


	/*
	  Constructor for initializing the memory and its size.
	  params:
			- memSize: Physical size of the memory.
			- stateSize: The size of each state.
			- nActions: The number of action taken given each state.
			- nPossibleActions: The number of possible actions that the agent can take each state.
	*/
	ACERReplayMemory(unsigned int memSize, unsigned int maxEpisodeLength, 
		unsigned int stateSize, unsigned int nActions, unsigned int nPossibleActions);

	/*
	  Stores input as data in memory.
	  params:
			- state: The current state.
			- action: The action(s) taken.
			- reward: The reward for taking that action.
			- newState: The state resulting from that action.
			- terminal: Whether the new state is terminal or not.
	*/
	void StoreStateTransition(
		State state,
		vector<float> actions,
		float reward,
		State newState,
		bool terminal,
		vector<float> actionProbabilities);

	/*
	  Samples an random part of the memory.
	  params:
			- batchSize: The size of the sample.
	  returns:
			- A memory sample object.
	*/
	std::vector<Trajectory> SampleMemory(unsigned int batchSize, unsigned int trajectoryLength);

	/*
	  Saves memory to multiple files. One for each tensor.
	  TODO: Dynamically create directory for the files to exist in.
	*/
	void SaveMemory();

	/*
	  Loads memory from multiple files. One for each tensor.
	  TODO: Error checking for if the files don't exist.
	*/
	void LoadMemory();

	/* Gets the current size of the memory (number of entries added). */
	inline unsigned int GetCurrentMemsize() { return trajectories.size(); };

private:

	/* Size of the memory. */
	unsigned int memSize;

	/* Max length of episode. */
	unsigned int maxEpisodeLength;

	/* Size of the state. */
	unsigned int stateSize;

	/* Number of actions taken in any given state. */
	unsigned int nActions;

	/* The number of possible actions that the agent can take each state. */
	unsigned int nPossibleActions;

	/* Memory */
	std::deque<Trajectory> trajectories;
	Trajectory currTrajectory;

	/* TODO: Temp way to save/load memCounter without implementing framework for it. */
	torch::Tensor memCounterMem;
};
