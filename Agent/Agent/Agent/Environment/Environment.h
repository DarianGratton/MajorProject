#pragma once

#include <vector>

#include "State.h"

using namespace std;

/*
  Environment

  ***Description***

  Author: Darian G.
*/
class Environment {
public:
	/*
	  Constructor for initializing the environment.
	*/
	Environment(State initState);

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
	inline void SetInitState(State state) { initState = state; };

	/*
	  Gets the current state of the environment.
	*/
	inline State GetCurrState() const { return currState; };
	
	/* 
	  Sets the current state of the environment and sets the previous 
	  state of the environment to the old state.
	*/
	inline void SetCurrState(State state) 
	{ 
		prevState = currState;
		currState = state; 
	};

	/* 
	  Gets the previous state of the environment.
	*/
	inline State GetPrevState() const { return prevState; };

	/*
	  Gets the previous action taken in the environment. 
	*/
	inline vector<float> GetAction() const { return action; };
	
	/*
	  Sets the previous action taken in the environment.
	*/
	inline void SetAction(vector<float> newAction) { action = newAction; };
	
	/*
	  Gets the previous reward earned in the environment.
	*/
	inline float GetReward() const { return reward; };
	
	/*
	  Sets the previous reward earned in the environment and added to
	  the total reward earned.
	*/
	inline void SetReward(float newReward) 
	{ 
		totalReward += newReward;
		reward = newReward; 
	};

	/*
	  Gets the total reward earned in the environment.
	*/
	inline float GetTotalReward() const { return totalReward; };
	
	/* 
	  Gets whether the environment is in a terminal state.
	*/
	inline bool IsTerminal() const { return isTerminal; };
	
	/*
	  Sets whether the environment is in a terminal state.
	*/
	inline void SetIsTerminal(bool terminal) { isTerminal = terminal; };

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
