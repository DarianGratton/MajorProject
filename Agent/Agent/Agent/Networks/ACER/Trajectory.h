#pragma once

#include <torch/torch.h>

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
		unsigned int nActions, unsigned int nPossibleActions)
	{
		states = torch::zeros({ maxEpisodeLength, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
		newStates = torch::zeros({ maxEpisodeLength, stateSize }, torch::TensorOptions().dtype(torch::kFloat32));
		actions = torch::zeros({ maxEpisodeLength, nActions }, torch::TensorOptions().dtype(torch::kFloat32));
		rewards = torch::zeros(maxEpisodeLength, torch::TensorOptions().dtype(torch::kFloat32));
		terminals = torch::zeros(maxEpisodeLength, torch::TensorOptions().dtype(torch::kBool));
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
	}

	/* */
	unsigned int numOfTransitions = 0;

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
