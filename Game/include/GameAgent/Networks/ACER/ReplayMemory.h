#pragma once

#include <torch/torch.h>

#include <deque>

#include "Trajectory.h"
#include "../../Environment/State.h"

/*
  ReplayMemory

  A class that defines the memory of the ACER Algorithm.
  Stores a list of trajectories which each contain the states, actions, rewards, etc. for a 
  single episode that the agent plays in a environment. 

  Note: State size and number of actions per state must remain the same as when initialized.

  TODO: Modify Memory Size to be based on number of transitions rather than Trajectories in memory.

  Author: Darian G.
*/
class ACERReplayMemory
{
public:


	/*
	  Constructor for initializing the memory and its size.
	  params:
			- memSize: Physical size of the memory.
			- maxEpisodeLength: Max length for a single episode in a environment.
			- stateSize: The size of each state.
			- nActions: The number of actions taken given each state.
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
			- policy: The probabilities of each possible action before the actual action was chosen.
	*/
	void StoreStateTransition(
		GameAgent::State state,
		std::vector<float> actions,
		float reward,
		GameAgent::State newState,
		bool terminal,
		std::vector<float> policy);

	/*
	  Samples an random part of the memory.
	  params:
			- batchSize: The size of the sample.
	  returns:
			- A memory sample object of size { trajectoryLength, batchSize }.
	*/
	std::vector<Trajectory> SampleMemory(unsigned int batchSize, unsigned int trajectoryLength);

	/* 
	  Gets the current size of the memory (number of entries added). 
	*/
	inline unsigned int GetCurrentMemsize() { return trajectories.size(); };

	/*
	  Gets the current trajectory being built.
	*/
	inline Trajectory GetCurrTrajectory() { return currTrajectory; };

	/*
	  Gets the previous trajectory built.
	*/
	inline Trajectory GetPrevTrajectory() { return prevTrajectory; };

private:

	/* Memory */
	std::deque<Trajectory> trajectories;

	/* The current trajectory being built before being placed in memory. */
	Trajectory currTrajectory;

	/* The previous trajectory built. */
	Trajectory prevTrajectory;

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
};
