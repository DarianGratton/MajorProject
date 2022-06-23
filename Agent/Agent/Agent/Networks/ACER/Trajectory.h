#pragma once

#include <torch/torch.h>

#include "../../Environment/State.h"

using namespace RLGameAgent;

/*
  Trajectory

  ***Description***

  Author: Darian G.
*/
struct Trajectory
{
	/* Default Constructor. */
	Trajectory() = default;

	/* Constructor. */
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

	void StoreTransition(
		State state,
		vector<int> newActions,
		float reward,
		State newState,
		bool terminal,
		vector<float> actionProbabilities)
	{
		/*states.slice(0, numOfTransitions, numOfTransitions + 1) = torch::nn::functional::normalize(
			torch::from_blob(state.ToVector().data(), { 1, stateSize }),
			torch::nn::functional::NormalizeFuncOptions().p(1).dim(1));*/

		states.slice(0, numOfTransitions, numOfTransitions + 1) = state.ToTensor();

		/*newStates.slice(0, numOfTransitions, numOfTransitions + 1) = torch::nn::functional::normalize(
			torch::from_blob(newState.ToVector().data(), { 1, stateSize }),
			torch::nn::functional::NormalizeFuncOptions().p(1).dim(1));;*/

		newStates.slice(0, numOfTransitions, numOfTransitions + 1) = newState.ToTensor();

		for (int i = 0; i < newActions.size(); i++)
			actions[numOfTransitions][i].data() = newActions.at(i);

		rewards[numOfTransitions].data() = reward;
		terminals[numOfTransitions].data() = terminal;

		for (int i = 0; i < actionProbabilities.size(); i++)
			policy[numOfTransitions][i].data() = actionProbabilities.at(i);

		numOfTransitions++;
	}

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

	/* */
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
