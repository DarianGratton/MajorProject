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
		cerr << "ReplayMemory() Error: maxEpisodeLength should not be greater than memSize. " << endl;
		
		// TODO: Replace with exception system
		return;
	}

	memSize = memSize / maxEpisodeLength;
	currTrajectory = Trajectory(maxEpisodeLength, stateSize, nActions, nPossibleActions);
}

void ACERReplayMemory::StoreStateTransition(
	State state,
	vector<float> actions,
	float reward,
	State newState,
	bool terminal,
	vector<float> policy)
{
	// Error checking
	if (state.Size() != stateSize)
	{
		// TODO: Replace with exception system
		cerr << "ReplayMemory StoreStateTransition() Error: State is of different size than when it was initialized, State size is: " << state.Size() << endl;
		return;
	}

	// Add elements to trajectory
	currTrajectory.StoreTransition(state, actions, reward, newState, terminal, policy);

	// Check if episode in environment has ended
	if (terminal || currTrajectory.numOfTransitions >= maxEpisodeLength)
	{
		// Insert elements
		if (trajectories.size() >= memSize)
		{
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
