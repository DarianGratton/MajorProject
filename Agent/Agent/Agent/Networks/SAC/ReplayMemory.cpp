#include "ReplayMemory.h"

#include <iostream>
#include <random>

ReplayMemory::ReplayMemory(unsigned int memSize, unsigned int nActions) : memSize(memSize)
{
	if (memSize == 0)
	{
		cerr << "Error: memSize should not be 0, defaulting to 1" << endl;
		memSize = 1;
	}
}

void ReplayMemory::StoreStateTransition(
	list<State> state,
	list<unsigned int> action,
	long reward,
	list<State> newState,
	bool terminal)
{
	// If memory is full then remove first element
	if (stateMem.size() >= memSize)
	{
		stateMem.erase(stateMem.begin());
		actionMem.erase(actionMem.begin());
		rewardMem.erase(rewardMem.begin());
		newStateMem.erase(newStateMem.begin());
		terminalMem.erase(terminalMem.begin());
	}

	// Insert elements
	stateMem.push_back(state);
	actionMem.push_back(action);
	rewardMem.push_back(reward);
	newStateMem.push_back(newState);
	terminalMem.push_back(terminal);
}

ReplayMemory::MemorySample ReplayMemory::SampleMemory(unsigned int batchSize)
{
	// Generate random sample
	unsigned int memCount = static_cast<unsigned int>(stateMem.size());
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, memCount - 1);

	// Get random sample
	MemorySample sample;
	for (int i = 0; i <= batchSize; ++i)
	{
		int batch = distrib(gen);
		sample.states.push_back(stateMem.at(batch));
		sample.newStates.push_back(newStateMem.at(batch));
		sample.actions.push_back(actionMem.at(batch));
		sample.rewards.push_back(rewardMem.at(batch));
		sample.terminals.push_back(terminalMem.at(batch));
	}

	// Return object
	return sample;
}

void ReplayMemory::SaveMemory()
{

}

void ReplayMemory::LoadMemory()
{

}
