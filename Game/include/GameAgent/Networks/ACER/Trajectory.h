#pragma once

#include <torch/torch.h>

#include "../../Environment/State.h"

/*
  Trajectory

  Stores the states, actions, rewards, new states, terminals, and policies
  for a single episode that the agent plays in a environment.

  Author: Darian G.
*/
struct Trajectory
{
	/* Default Constructor. */
	Trajectory() 
	{
		auto options = torch::TensorOptions().dtype(torch::kFloat32);
		states = torch::zeros({ 1, 1 }, options);
		newStates = torch::zeros({ 1, 1 }, options);
		actions = torch::zeros({ 1, 1 }, options);
		rewards = torch::zeros({ 1, 1 }, options);
		terminals = torch::zeros({ 1, 1 }, options);
		policy = torch::zeros({ 1, 1 }, options);

		numOfTransitions = 0;
	}
	/* 
	  Constructor for initializing trajectory with a set size.
	  params:
			- maxEpisodeLength: Max length for a single episode in a environment.
			- stateSize: The size of each size.
			- nActions: The number of actions taken each state.
			- nPossibleActions: The number of actions taken each state.
	*/
	Trajectory(unsigned int maxEpisodeLength, unsigned int stateSize,
		unsigned int nActions, unsigned int nPossibleActions) :
		stateSize(stateSize)
	{
		auto options = torch::TensorOptions().dtype(torch::kFloat32);
		states = torch::zeros({ maxEpisodeLength, stateSize }, options);
		newStates = torch::zeros({ maxEpisodeLength, stateSize }, options);
		actions = torch::zeros({ maxEpisodeLength, nActions }, options);
		rewards = torch::zeros({ maxEpisodeLength, 1 }, options);
		terminals = torch::zeros({ maxEpisodeLength, 1 }, options);
		policy = torch::zeros({ maxEpisodeLength, nPossibleActions }, options);

		numOfTransitions = 0;
	}

	/* Copy Constructor. */
	Trajectory(const Trajectory& trajectory)
	{
		states    = trajectory.states.clone();
		actions	  = trajectory.actions.clone();
		rewards	  = trajectory.rewards.clone();
		newStates = trajectory.newStates.clone();
		terminals = trajectory.terminals.clone();
		policy	  = trajectory.policy.clone();

		numOfTransitions = trajectory.numOfTransitions;
		stateSize = trajectory.stateSize;
	}

	/* 
	  Stores input as a transition in trajectory.
	  params:
			- state: The current state.
			- action: The action(s) taken.
			- reward: The reward for taking that action.
			- newState: The state resulting from that action.
			- terminal: Whether the new state is terminal or not.
			- policy: The probabilities of each possible action before the actual action was chosen.
	*/
	void StoreTransition(
		GameAgent::State state,
		std::vector<float> newActions,
		float reward,
		GameAgent::State newState,
		bool terminal,
		std::vector<float> actionProbabilities)
	{
		states.slice(0, numOfTransitions, numOfTransitions + 1) = state.ToTensor();
		newStates.slice(0, numOfTransitions, numOfTransitions + 1) = newState.ToTensor();

		for (int i = 0; i < newActions.size(); i++)
			actions[numOfTransitions][i].data() = newActions.at(i);

		rewards[numOfTransitions].data() = reward;
		terminals[numOfTransitions].data() = terminal;

		for (int i = 0; i < actionProbabilities.size(); i++)
			policy[numOfTransitions][i].data() = actionProbabilities.at(i);

		numOfTransitions++;
	}

	/* 
	  Truncates all empty transitions from the trajectory.
	*/
	void Truncate()
	{
		int64_t end = numOfTransitions;
		states = states.index({ torch::indexing::Slice(torch::indexing::None, end), "..." });
		newStates = newStates.index({ torch::indexing::Slice(torch::indexing::None, end), "..." });
		actions = actions.index({ torch::indexing::Slice(torch::indexing::None, end), "..." });
		rewards = rewards.index({ torch::indexing::Slice(torch::indexing::None, end), "..." });
		terminals = terminals.index({ torch::indexing::Slice(torch::indexing::None, end), "..." });
		policy = policy.index({ torch::indexing::Slice(torch::indexing::None, end), "..." });
	}

	/* The number of transitions added to the trajectory. */
	unsigned int numOfTransitions = 0;

	/* State Size. */
	unsigned int stateSize = 0;

	/* Current state of environment. */
	torch::Tensor states;
	
	/* Action chosen in current state. */
	torch::Tensor actions;
	
	/* Reward earned. */
	torch::Tensor rewards;
	
	/* State resulting from action. */
	torch::Tensor newStates;
	
	/* States whether new state discovered is terminal state. */
	torch::Tensor terminals;
	
	/* Probabilities of the actions chosen by the behaviour policy (mu(*|x_i)) */
	torch::Tensor policy;
};
