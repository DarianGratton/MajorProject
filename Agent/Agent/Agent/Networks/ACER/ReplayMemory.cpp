#include "ReplayMemory.h"

#include "RandomIterator.h"

#include <iostream>
#include <random>

ACERReplayMemory::ACERReplayMemory(
	unsigned int memSize, unsigned int stateSize, 
	unsigned int nActions, unsigned int nPossibleActions)
	: memSize(memSize), stateSize(stateSize), 
	nActions(nActions), nPossibleActions(nPossibleActions)
{
	// Error checking memory size
	if (memSize == 0)
	{
		cerr << "Error: memSize should not be 0, defaulting to 1" << endl;
		memSize = 1;
	}

	// Initializing memory
	stateMem = torch::zeros({ memSize, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
	newStateMem = torch::zeros({ memSize, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
	actionMem = torch::zeros({ memSize, nActions }, torch::TensorOptions().dtype(torch::kFloat32));
	rewardMem = torch::zeros(memSize, torch::TensorOptions().dtype(torch::kFloat32));
	terminalMem = torch::zeros(memSize, torch::TensorOptions().dtype(torch::kBool));
	actionProbabilitiesMem = torch::zeros({ memSize, nPossibleActions }, torch::TensorOptions().dtype(torch::kFloat32));
	memCounterMem = torch::zeros(1, torch::TensorOptions().dtype(torch::kInt32));

	// Initializing variables
	memCounter = 0;
	currMemStored = 0;
}

void ACERReplayMemory::StoreStateTransition(
	State state,
	vector<float> actions,
	float reward,
	State newState,
	bool terminal,
	vector<float> actionProbabilities)
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

	for (int i = 0; i < actions.size(); i++)
		actionProbabilitiesMem[memCounter][i].data() = actionProbabilities.at(i);

	// Increment counter
	memCounter++;

	if (currMemStored != memSize)
		currMemStored++;
}

ACERReplayMemory::MemorySample ACERReplayMemory::SampleMemory(unsigned int batchSize)
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
	sample.actionProbabilities = torch::zeros({ batchSize, nPossibleActions }, torch::TensorOptions().dtype(torch::kFloat32));

	// Get random sample
	for (int i = 0; i < batchSize; ++i)
	{
		int batch = iterator.next() - 1;
		sample.states[i].data() = stateMem[batch];
		sample.newStates[i].data() = newStateMem[batch];
		sample.actions[i].data() = actionMem[batch];
		sample.rewards[i].data() = rewardMem[batch];
		sample.terminals[i].data() = terminalMem[batch];
		sample.actionProbabilities[i].data() = actionProbabilitiesMem[batch];
	}

	// Return object
	return sample;
}

void ACERReplayMemory::SaveMemory()
{
	torch::save(stateMem, "states.pt");
	torch::save(newStateMem, "newStates.pt");
	torch::save(actionMem, "actions.pt");
	torch::save(rewardMem, "rewards.pt");
	torch::save(terminalMem, "terminals.pt");

	// Save memCounter (temp)
	memCounterMem[0].data() = static_cast<int>(memCounter);
	torch::save(memCounterMem, "memCounter.pt");
}

void ACERReplayMemory::LoadMemory()
{
	torch::load(stateMem, "SACMemory/states.pt");
	torch::load(newStateMem, "SACMemory/newStates.pt");
	torch::load(actionMem, "SACMemory/actions.pt");
	torch::load(rewardMem, "SACMemory/rewards.pt");
	torch::load(terminalMem, "SACMemory/terminals.pt");

	// Load memCounter (temp)
	torch::load(memCounterMem, "memCounter.pt");
	memCounter = memCounterMem[0].item<int>();
}
