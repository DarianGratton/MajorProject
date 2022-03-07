#include "ReplayMemory.h"

#include "RandomIterator.h"

#include <iostream>
#include <random>

ReplayMemory::ReplayMemory(
	unsigned int memSize, unsigned int stateSize, unsigned int nActions) 
	: memSize(memSize), stateSize(stateSize), nActions(nActions)
{
	// Error checking memory size
	if (memSize == 0)
	{
		cerr << "Error: memSize should not be 0, defaulting to 1" << endl;
		memSize = 1;
	}

	// Initializing memory
	stateMem	= torch::zeros({ memSize, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
	newStateMem = torch::zeros({ memSize, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
	actionMem	= torch::zeros({ memSize, nActions }, torch::TensorOptions().dtype(torch::kFloat32));
	rewardMem	= torch::zeros(memSize, torch::TensorOptions().dtype(torch::kFloat32));
	terminalMem = torch::zeros(memSize, torch::TensorOptions().dtype(torch::kBool));

	// Initializing variables
	memCounter = 0;
	currMemStored = 0;
}

void ReplayMemory::StoreStateTransition(
	State state,
	vector<float> actions,
	float reward,
	State newState,
	bool terminal)
{
	// Error checking
	if (state.Size() != stateSize)
	{
		cerr << "ReplayMemory Error: State is of different size than when it was initialized, State size is: " << state.Size() << endl;
		return;
	}

	// If memory is full then reset memory counter
	if (memCounter >= memSize)
	{
		memCounter = 0;
	}

	// Insert elements
	stateMem.slice(0, memCounter, memCounter + 1) = torch::from_blob(state.ToVector().data(), { 1, stateSize });
	newStateMem.slice(0, memCounter, memCounter + 1) = torch::from_blob(newState.ToVector().data(), { 1, stateSize });
	
	for (int i = 0; i < actions.size(); i++)
		actionMem[memCounter][i].data() = actions.at(i);

	rewardMem[memCounter].data() = reward;
	terminalMem[memCounter].data() = terminal;

	// Increment counter
	memCounter++;

	if (currMemStored != memSize)
		currMemStored++;
}

ReplayMemory::MemorySample ReplayMemory::SampleMemory(unsigned int batchSize)
{
	// Generate random sample
	RandomIterator iterator(batchSize, 0, currMemStored - 1);

	// Initialize sample
	MemorySample sample;
	sample.states = torch::zeros({ batchSize, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
	sample.newStates = torch::zeros({ batchSize, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
	sample.actions = torch::zeros({ batchSize, nActions }, torch::TensorOptions().dtype(torch::kFloat32));
	sample.rewards = torch::zeros(batchSize, torch::TensorOptions().dtype(torch::kFloat32));
	sample.terminals = torch::zeros(batchSize, torch::TensorOptions().dtype(torch::kBool));

	// Get random sample
	for (int i = 0; i < batchSize; ++i)
	{
		int batch = iterator.next() - 1;
		sample.states[i].data() = stateMem[batch];
		sample.newStates[i].data() = newStateMem[batch];
		sample.actions[i].data() = actionMem[batch];
		sample.rewards[i].data() = rewardMem[batch];
		sample.terminals[i].data() = terminalMem[batch];
	}

	// Return object
	return sample;
}

void ReplayMemory::SaveMemory()
{
	torch::save(stateMem, "SACMemory/states.pt");
	torch::save(newStateMem, "SACMemory/newStates.pt");
	torch::save(actionMem, "SACMemory/actions.pt");
	torch::save(rewardMem, "SACMemory/rewards.pt");
	torch::save(terminalMem, "SACMemory/terminals.pt");
}

void ReplayMemory::LoadMemory()
{
	torch::load(stateMem, "SACMemory/states.pt");
	torch::load(newStateMem, "SACMemory/newStates.pt");
	torch::load(actionMem, "SACMemory/actions.pt");
	torch::load(rewardMem, "SACMemory/rewards.pt");
	torch::load(terminalMem, "SACMemory/terminals.pt");

	// TODO: Shouldn't be a issue for now but should fix
	memCounter = 0;
}
