#pragma once

#include <vector>

#include "State.h"

namespace GameAgent
{

/*
  Environment

  A class that defines a game environment and all its variables.

  Author: Darian G.
*/
class Environment 
{
public:

	/*
	  Constructor for initializing the environment.
	*/
	Environment(State initState);

	/*
	  Updates the environment's variables.
	  params:
			- newAction: New action taken in the environment.
			- newReward: New reward earned from taking action.
			- newState: New state reached from taking action.
			- terminal: Whether new state is terminal or not.
	*/
	void Update(std::vector<float> newAction, float newReward,
				State newState, bool terminal = false);

	/*
	  Resets the environment's variables for a new episode.
	*/
	void Reset();

	/*
	  Gets the initial state of the environment.
	*/
	inline State GetInitState() { return initState; }
	
	/* 
	  Sets the initial state of the environment.
	*/
	inline void SetInitState(State state) 
	{ 
		if (prevState.IsEmpty())
		{
			initState = state;
			currState = initState;
		}
		else
			std::cerr << "Environment::SetInitState(): Cannot change initial state after acting on the environment." << std::endl;
	}

	/*
	  Gets the current state of the environment.
	*/
	inline State GetCurrState() const { return currState; }

	/* 
	  Gets the previous state of the environment.
	*/
	inline State GetPrevState() const { return prevState; }

	/*
	  Gets the previous action taken in the environment. 
	*/
	inline std::vector<float> GetAction() const { return action; }
	
	/*
	  Gets the previous reward earned in the environment.
	*/
	inline float GetReward() const { return reward; }

	/*
	  Gets the total reward earned in the environment.
	*/
	inline float GetTotalReward() const { return totalReward; }

	/* 
	  Gets the history of reward earned in the environment.
	*/
	inline std::vector<float> GetRewardHistory() const { return rewardHistory; }
	
	/* 
	  Gets whether the environment is in a terminal state.
	*/
	inline bool IsTerminal() const { return isTerminal; }

	/* 
	  Gets the number of steps taken in the environment. 
	*/
	inline unsigned int GetSteps() const { return steps; }

private:
	
	/* The initial state of the environment. */
	State initState;

	/* The current state of the environment. */
	State currState;

	/* The prev state of the environment. */
	State prevState;

	/* The action previously taken in the environment. */
	std::vector<float> action;

	/* The reward previously earned in the environment. */
	float reward;

	/* The total rewards earned. */
	float totalReward;

	/* The history of rewards earned in the environment. */
	std::vector<float> rewardHistory;

	/* Is the current state of the environment a terminal state. */
	bool isTerminal;

	/* The number of steps taken in the environment. */
	unsigned int steps;
};

} /* namespace GameAgent */
