#pragma once

#include <list>
#include <vector>
#include <torch/torch.h>

#include "../../Environment/Reward.h"
#include "../../Environment/State.h"

using namespace std;

// TODO: Implement functionality for a basic SACAgent
class ReplayMemory
{
public:

	ReplayMemory(unsigned int memSize, unsigned int nActions);

	// Struct
	struct MemorySample
	{
		vector<list<State>> states;
		vector<list<unsigned int>> actions;
		vector<long> rewards;
		vector<list<State>> newStates;
		vector<bool> terminals;
	};

	// Casting might cause overflow so it suggests just using the size_t
	inline size_t GetCurrentMemsize() { return stateMem.size(); };

private:
	unsigned int memSize;
	
	// Memory
	vector<list<State>> stateMem;       
	vector<list<State>> newStateMem;    
	vector<list<unsigned int>> actionMem;
	vector<long> rewardMem;					// Reward should be a list of reward objects
	vector<bool> terminalMem;

public:
	void StoreStateTransition(
		list<State> state,
		list<unsigned int> action,
		long reward,
		list<State> newState,
		bool terminal);

	MemorySample SampleMemory(unsigned int batchSize);

	void SaveMemory();
	void LoadMemory();
};
