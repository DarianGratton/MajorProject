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
		states = torch::zeros({ maxEpisodeLength, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
		newStates = torch::zeros({ maxEpisodeLength, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
		actions = torch::zeros({ maxEpisodeLength, nActions }, torch::TensorOptions().dtype(torch::kFloat32));
		rewards = torch::zeros({ maxEpisodeLength, 1 }, torch::TensorOptions().dtype(torch::kFloat32));
		terminals = torch::zeros({ maxEpisodeLength, 1 }, torch::TensorOptions().dtype(torch::kFloat32));
		policy = torch::zeros({ maxEpisodeLength, nPossibleActions }, torch::TensorOptions().dtype(torch::kFloat32));
	
		numOfTransitions = 0;
	}

	/* Copy Constructor. */
	Trajectory(const Trajectory& trajectory)
	{
		states    = trajectory.states.clone().detach();
		actions	  = trajectory.actions.clone().detach();
		rewards	  = trajectory.rewards.clone().detach();
		newStates = trajectory.newStates.clone().detach();
		terminals = trajectory.terminals.clone().detach();
		policy	  = trajectory.policy.clone().detach();

		numOfTransitions = trajectory.numOfTransitions;
		stateSize = trajectory.stateSize;
	}

	void StoreTransition(
		State state,
		vector<float> newActions,
		float reward,
		State newState,
		bool terminal,
		vector<float> actionProbabilities)
	{
		states.slice(0, numOfTransitions, numOfTransitions + 1) = torch::from_blob(state.ToVector().data(), { 1, stateSize });
		newStates.slice(0, numOfTransitions, numOfTransitions + 1) = torch::from_blob(newState.ToVector().data(), { 1, stateSize });

		for (int i = 0; i < newActions.size(); i++)
			actions[numOfTransitions][i].data() = newActions.at(i);

		rewards[numOfTransitions].data() = reward;
		terminals[numOfTransitions].data() = terminal;

		for (int i = 0; i < actionProbabilities.size(); i++)
			policy[numOfTransitions][i].data() = actionProbabilities.at(i);

		numOfTransitions++;
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
