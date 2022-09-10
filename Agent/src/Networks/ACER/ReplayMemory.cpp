#include "ReplayMemory.h"

#include "RandomIterator.h"

#include <iostream>
#include <limits>
#include <random>

ACERReplayMemory::ACERReplayMemory(
	unsigned int memSize, 
	unsigned int maxEpisodeLength, unsigned int stateSize,
	unsigned int nActions, unsigned int nPossibleActions)
	: memSize(memSize), 
	maxEpisodeLength(maxEpisodeLength), stateSize(stateSize), 
	nActions(nActions), nPossibleActions(nPossibleActions)  
{
	if (memSize < maxEpisodeLength)
	{
		std::cerr << "ReplayMemory() Error: maxEpisodeLength should not be greater than memSize. " << std::endl;
		
		// TODO: Replace with exception system
		return;
	}

	// memSize = memSize / maxEpisodeLength;
	currMemSize = 0;
	currTrajectory = Trajectory(maxEpisodeLength, stateSize, nActions, nPossibleActions);
}

void ACERReplayMemory::StoreStateTransition(
	GameAgent::State state,
	std::vector<float> actions,
	float reward,
	GameAgent::State newState,
	bool terminal,
	std::vector<float> policy)
{
	// Error checking
	if (state.Size() != stateSize)
	{
		// TODO: Replace with exception system
		std::cerr << "ReplayMemory StoreStateTransition() Error: State is of different size than when it was initialized, State size is: " << state.Size() << std::endl;
		return;
	}

	// Add elements to trajectory
	currTrajectory.StoreTransition(state, actions, reward, newState, terminal, policy);

	// Check if episode in environment has ended
	if (terminal || currTrajectory.numOfTransitions >= maxEpisodeLength)
	{
		// Truncate trajectory to clear zeros and update Current Memmory Size
		currTrajectory.Truncate();
		currMemSize += currTrajectory.numOfTransitions;

		// Insert elements
		if (currMemSize >= memSize)
		{
			currMemSize -= trajectories.at(0).numOfTransitions;
			trajectories.pop_back(); 
			trajectories.push_front(currTrajectory);
		}
		else
		{
			trajectories.push_back(currTrajectory);
		}
		
		// Reset currTrajectory
		prevTrajectory = currTrajectory;
		currTrajectory = Trajectory(maxEpisodeLength, stateSize, nActions, nPossibleActions);
	}
}

std::vector<Trajectory> ACERReplayMemory::SampleMemory(unsigned int batchSize, unsigned int trajectoryLength)
{
	// Generate random indices
	RandomIterator indices(batchSize, 0, trajectories.size() - 1);

	// Sample Trajectories
	std::vector<Trajectory> sampledTrajectories;
	for (int i = 0; i < batchSize; ++i)
	{
		int index = indices.next() - 1;
		Trajectory storedTrajectory(trajectories[index]);

		RandomIterator iterator(1, 0, storedTrajectory.numOfTransitions / 2);
		int start = iterator.next() - 1;
		int end = (start + trajectoryLength >= storedTrajectory.numOfTransitions) ? storedTrajectory.numOfTransitions : start + trajectoryLength;

		storedTrajectory.states		= storedTrajectory.states.index({ torch::indexing::Slice(start, end), "..." });
		storedTrajectory.newStates	= storedTrajectory.newStates.index({ torch::indexing::Slice(start, end), "..." });
		storedTrajectory.actions	= storedTrajectory.actions.index({ torch::indexing::Slice(start, end), "..." });
		storedTrajectory.rewards	= storedTrajectory.rewards.index({ torch::indexing::Slice(start, end), "..." });
		storedTrajectory.terminals	= storedTrajectory.terminals.index({ torch::indexing::Slice(start, end), "..." });
		storedTrajectory.policy		= storedTrajectory.policy.index({ torch::indexing::Slice(start, end), "..." });
	
		sampledTrajectories.push_back(storedTrajectory);
	}

	// Truncate longer trajectories so they are all the same size
	int64_t smallestTrajectory = std::numeric_limits<int64_t>::max();
	for (Trajectory trajectory : sampledTrajectories)
		if (trajectory.states.size(0) < smallestTrajectory)
			smallestTrajectory = trajectory.states.size(0);

	for (Trajectory& trajectory : sampledTrajectories)
	{
		int64_t start = trajectory.states.size(0) - smallestTrajectory;
		trajectory.states = trajectory.states.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.newStates = trajectory.newStates.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.actions = trajectory.actions.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.rewards = trajectory.rewards.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.terminals = trajectory.terminals.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.policy = trajectory.policy.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
	}

	// Batch trajectories
	std::vector<Trajectory> batchedTrajectories;
	for (int i = 0; i < sampledTrajectories[0].states.size(0); i++)
	{
		Trajectory batchedTrajectory(batchSize, stateSize, nActions, nPossibleActions);
		for (int j = 0; j < batchSize; j++)
		{
			batchedTrajectory.states[j].data() = sampledTrajectories[j].states[i].data();
			batchedTrajectory.newStates[j].data() = sampledTrajectories[j].newStates[i].data();
			batchedTrajectory.actions[j].data() = sampledTrajectories[j].actions[i].data();
			batchedTrajectory.rewards[j].data() = sampledTrajectories[j].rewards[i].data();
			batchedTrajectory.terminals[j].data() = sampledTrajectories[j].terminals[i].data();
			batchedTrajectory.policy[j].data() = sampledTrajectories[j].policy[i].data();
		}

		batchedTrajectories.push_back(batchedTrajectory);
	}

	// Return object
	return batchedTrajectories;
}

void ACERReplayMemory::Save()
{
	// Initialize storage for Memory Data (CurrMemSize and Trajectory Sizes)
	int memDataSize = trajectories.size() + 1;
	torch::Tensor memData = torch::zeros({ memDataSize }, torch::kInt32);
	memData[0].data() = static_cast<int>(currMemSize);

	std::cout << std::endl;
	std::cout << "Trajectories Stored in Memory: " << trajectories.size() << std::endl;
	std::cout << "Memory Size: " << currMemSize << std::endl;
	std::cout << "Trajectory Size: " << trajectories[0].states.size(0) << std::endl;

	// Batch Trajectories
	Trajectory batchedTrajectory = trajectories.at(0);
	memData[1].data() = static_cast<int>(trajectories.at(0).numOfTransitions);
	for (int i = 1; i < trajectories.size(); i++)
	{
		Trajectory storedTrajectory = trajectories.at(i);
		batchedTrajectory.states = torch::cat({batchedTrajectory.states, storedTrajectory.states}, 0);
		batchedTrajectory.newStates = torch::cat({batchedTrajectory.newStates, storedTrajectory.newStates}, 0);
		batchedTrajectory.actions = torch::cat({batchedTrajectory.actions, storedTrajectory.actions}, 0);
		batchedTrajectory.rewards = torch::cat({batchedTrajectory.rewards, storedTrajectory.rewards}, 0);
		batchedTrajectory.terminals = torch::cat({batchedTrajectory.terminals, storedTrajectory.terminals}, 0);
		batchedTrajectory.policy = torch::cat({batchedTrajectory.policy, storedTrajectory.policy}, 0);
		
		int index = i + 1;
		memData[index].data() = static_cast<int>(storedTrajectory.numOfTransitions);
	}

	std::cout << "MemData: " << memData << std::endl;

	// Save Tensors
	torch::save(batchedTrajectory.states,	 "ACERMemoryStates.pt");
	torch::save(batchedTrajectory.newStates, "ACERMemoryNewStates.pt");
	torch::save(batchedTrajectory.actions,	 "ACERMemoryActions.pt");
	torch::save(batchedTrajectory.rewards,	 "ACERMemoryRewards.pt");
	torch::save(batchedTrajectory.terminals, "ACERMemoryTerminals.pt");
	torch::save(batchedTrajectory.policy,	 "ACERMemoryPolicy.pt");
	torch::save(memData,					 "ACERMemoryData.pt");
}

void ACERReplayMemory::Load()
{
	// Initialize Tensors
	Trajectory savedTrajectory;
	torch::Tensor memData;

	// Load Saved Memory
	try
	{
		torch::load(savedTrajectory.states,    "ACERMemoryStates.pt");
		torch::load(savedTrajectory.newStates, "ACERMemoryNewStates.pt");
		torch::load(savedTrajectory.actions,   "ACERMemoryActions.pt");
		torch::load(savedTrajectory.rewards,   "ACERMemoryRewards.pt");
		torch::load(savedTrajectory.terminals, "ACERMemoryTerminals.pt");
		torch::load(savedTrajectory.policy,    "ACERMemoryPolicy.pt");
		torch::load(memData,				   "ACERMemoryData.pt");
	}
	catch (const c10::Error& e)
	{
		// File not found
		std::cout << "ReplayMemory::Load(): Save files not found to load." << std::endl;
		return;
	}

	// Clear Current Memory
	trajectories.clear();

	// Parse saved data into Memory
	currMemSize = memData[0].item<int>();
	int64_t start = 0;
	for (int i = 1; i < memData.size(0); i++)
	{
		Trajectory trajectory;
		trajectory.numOfTransitions = memData[i].item<int>();

		int64_t end = start + trajectory.numOfTransitions;
		trajectory.states = savedTrajectory.states.index({ torch::indexing::Slice(start, end) , "..." });
		trajectory.newStates = savedTrajectory.newStates.index({ torch::indexing::Slice(start, end) , "..." });
		trajectory.actions = savedTrajectory.actions.index({ torch::indexing::Slice(start, end) , "..." });
		trajectory.rewards = savedTrajectory.rewards.index({ torch::indexing::Slice(start, end) , "..." });
		trajectory.terminals = savedTrajectory.terminals.index({ torch::indexing::Slice(start, end) , "..." });
		trajectory.policy = savedTrajectory.policy.index({ torch::indexing::Slice(start, end) , "..." });
		start += trajectory.numOfTransitions;

		trajectories.push_back(trajectory);
	}

	std::cout << "Trajectories Size after Load: " << trajectories.size() << std::endl;
}
