#pragma once

#include <vector>

#include "State.h"

using namespace std;

/*
  Environment

  A class that defines a game environment and all its variables.

  Author: Darian G.
*/
class Environment {
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
	void Update(vector<float> newAction, float newReward, 
				State newState, bool terminal = false);

	/*
	  Resets the environment's variables for a new episode.
	*/
	void Reset();
	
	/* 
	  Sets the initial state of the environment.
	  TODO: Return to this when doing the state storage as changing it 
			while playing in the environment might cause issues.
			Might be able to set it to only change when isTerminal is 
			true or a flag is set during reset.
	*/
	inline void SetInitState(State state) { initState = state; }

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
	inline vector<float> GetAction() const { return action; }
	
	/*
	  Gets the previous reward earned in the environment.
	*/
	inline float GetReward() const { return reward; }

	/*
	  Gets the total reward earned in the environment.
	*/
	inline float GetTotalReward() const { return totalReward; }
	
	/* 
	  Gets whether the environment is in a terminal state.
	*/
	inline bool IsTerminal() const { return isTerminal; }

private:
	
	/* The initial state of the environment. */
	State initState;

	/* The current state of the environment. */
	State currState;

	/* The prev state of the environment. */
	State prevState;

	/* The action previously taken in the environment. */
	vector<float> action; 

	/* The reward previously earned in the environment. */
	float reward;

	/* The total rewards earned. */
	float totalReward;

	/* Is the current state of the environment a terminal state. */
	bool isTerminal;
};
