#pragma once

#include <torch/torch.h>

#include <vector>

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
	ACERReplayMemory(unsigned int memSize, unsigned int stateSize, unsigned int nActions, unsigned int nPossibleActions);

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

	/* Defines what a sample of memory contains. */
	struct MemorySample
	{
		torch::Tensor states;
		torch::Tensor actions;
		torch::Tensor rewards;
		torch::Tensor newStates;
		torch::Tensor terminals;
		torch::Tensor actionProbabilities;
	};

	/*
	  Samples an random part of the memory.
	  params:
			- batchSize: The size of the sample.
	  returns:
			- A memory sample object.
	*/
	MemorySample SampleMemory(unsigned int batchSize);

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
	inline unsigned int GetCurrentMemsize() { return currMemStored; };

private:

	/* Size of the memory. */
	unsigned int memSize;

	/* Number of entries added to memory (max: memSize). */
	unsigned int currMemStored;

	/* Current position of memory. */
	unsigned int memCounter;

	/* Size of the state. */
	unsigned int stateSize;

	/* Number of actions taken in any given state. */
	unsigned int nActions;

	/* The number of possible actions that the agent can take each state. */
	unsigned int nPossibleActions;

	/* Memory */
	torch::Tensor stateMem;
	torch::Tensor newStateMem;
	torch::Tensor actionMem;
	torch::Tensor rewardMem;
	torch::Tensor terminalMem;
	torch::Tensor actionProbabilitiesMem;

	/* TODO: Temp way to save/load memCounter without implementing framework for it. */
	torch::Tensor memCounterMem;
};
