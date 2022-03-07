#pragma once

#include <torch/torch.h>

#include <vector>

#include "../../Environment/Reward.h"
#include "../../Environment/State.h"

using namespace std;

class ReplayMemory
{
public:
	ReplayMemory(unsigned int memSize, unsigned int stateSize, unsigned int nActions);

	// Struct
	struct MemorySample
	{
		torch::Tensor states;
		torch::Tensor actions;
		torch::Tensor rewards;
		torch::Tensor newStates;
		torch::Tensor terminals;
	};

	void StoreStateTransition(
		State state,
		vector<float> actions,
		float reward,
		State newState,
		bool terminal);

	MemorySample SampleMemory(unsigned int batchSize);

	void SaveMemory();
	void LoadMemory();

	inline unsigned int GetCurrentMemsize() { return memCounter; };

private:
	unsigned int memSize;
	unsigned int currMemStored;
	unsigned int memCounter;
	unsigned int stateSize;
	unsigned int nActions;
	
	// Memory
	torch::Tensor stateMem;     // Stores State Objects
	torch::Tensor newStateMem;  // Stores State Objects
	torch::Tensor actionMem;    // Stores Integers
	torch::Tensor rewardMem;	// Reward should be a list of reward objects
	torch::Tensor terminalMem;  // Stores Booleans
};
