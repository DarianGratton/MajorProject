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
		// TODO: Throw error
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
	vector<float> actionProbabilities)
{
	// Error checking
	if (state.Size() != stateSize)
	{
		cerr << "ReplayMemory: StoreStateTransition Error: State is of different size than when it was initialized, State size is: " << state.Size() << endl;
		return;
	}

	// Add elements to trajectory
	currTrajectory.states.slice(0, currTrajectory.numOfTransitions, currTrajectory.numOfTransitions + 1) = torch::from_blob(state.ToVector().data(), { 1, stateSize });
	currTrajectory.newStates.slice(0, currTrajectory.numOfTransitions, currTrajectory.numOfTransitions + 1) = torch::from_blob(newState.ToVector().data(), { 1, stateSize });

	for (int i = 0; i < actions.size(); i++)
		currTrajectory.actions[trajectories.size() - 1][i].data() = actions.at(i);

	currTrajectory.rewards[currTrajectory.numOfTransitions].data() = reward;
	currTrajectory.terminals[currTrajectory.numOfTransitions].data() = terminal;

	for (int i = 0; i < actions.size(); i++)
		currTrajectory.policy[currTrajectory.numOfTransitions][i].data() = actionProbabilities.at(i);

	// Increment counter
	currTrajectory.numOfTransitions++;

	// Check if trajectory has ended
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
		currTrajectory = Trajectory(maxEpisodeLength, stateSize, nActions, nPossibleActions);
	}
}

std::vector<Trajectory> ACERReplayMemory::SampleMemory(unsigned int batchSize, unsigned int trajectoryLength)
{
	// Generate random indices
	RandomIterator indices(batchSize, 0, trajectories.size() - 1);

	// Sample Trajectories
	// TODO: Test
	// TODO: Fix bug with start + trajectoryLength
	// TODO: Fix bug with start equal to size of trajectory (meaning we get 0 in the trajectory)
	std::vector<Trajectory> sampledTrajectories;
	for (int i = 0; i < batchSize; ++i)
	{
		int index = indices.next() - 1;
		Trajectory storedTrajectory(trajectories[index]);

		RandomIterator iterator(1, 0, storedTrajectory.numOfTransitions - 1);
		int start = iterator.next() - 1;
		
		storedTrajectory.states		= storedTrajectory.states.index({ torch::indexing::Slice(start, start + trajectoryLength), "..." });
		storedTrajectory.newStates	= storedTrajectory.newStates.index({ torch::indexing::Slice(start, start + trajectoryLength), "..." });
		storedTrajectory.actions	= storedTrajectory.actions.index({ torch::indexing::Slice(start, start + trajectoryLength), "..." });
		storedTrajectory.rewards	= storedTrajectory.rewards.index({ torch::indexing::Slice(start, start + trajectoryLength), "..." });
		storedTrajectory.terminals	= storedTrajectory.terminals.index({ torch::indexing::Slice(start, start + trajectoryLength), "..." });
		storedTrajectory.policy		= storedTrajectory.policy.index({ torch::indexing::Slice(start, start + trajectoryLength), "..." });
	
		sampledTrajectories.push_back(storedTrajectory);
	}

	// Truncate longer trajectories so they are all the same size
	// TODO: Test
	int64_t smallestTrajectory = std::numeric_limits<int64_t>::max();
	for (auto trajectory : sampledTrajectories)
		if (trajectory.states.size(0) < smallestTrajectory)
			smallestTrajectory = trajectory.states.size(0);

	for (auto trajectory : sampledTrajectories)
	{
		int64_t start = trajectory.states.size(0) - smallestTrajectory - 1;
		trajectory.states = trajectory.states.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.newStates = trajectory.newStates.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.actions = trajectory.actions.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.rewards = trajectory.rewards.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.terminals = trajectory.terminals.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
		trajectory.policy = trajectory.policy.index({ torch::indexing::Slice(start, torch::indexing::None), "..." });
	}

	// Batch Trajectories
	// TODO: Test
	std::vector<Trajectory> batchedTrajectories;
	for (int i = 0; i < sampledTrajectories.size(); i++)
	{
		Trajectory batchedTrajectory(sampledTrajectories.size(), stateSize, nActions, nPossibleActions);
		for (int j = 0; j < sampledTrajectories.size(); j++)
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

void ACERReplayMemory::SaveMemory()
{
	
}

void ACERReplayMemory::LoadMemory()
{
	
}
