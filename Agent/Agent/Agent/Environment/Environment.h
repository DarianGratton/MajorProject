#pragma once

#include <vector>

#include "State.h"

using namespace std;

// TODO: Template is a work around for Reward type and variable, should figure out better way
// - Overloaded functions might work with multiple reward variables
/// <summary>
/// 
/// </summary>
class Environment {
public:
	/*
	  Constructor.
	*/
	Environment();

	/*
	
	*/
	void Reset();
	
	/* 
	
	*/
	inline void SetInitState(State state) { initState = state; };

	/*
	
	*/
	inline State GetCurrState() const { return currState; };
	
	/* 
	
	*/
	inline void SetCurrState(State state) 
	{ 
		prevState = currState;
		currState = state; 
	};

	/* 
	
	*/
	inline State GetPrevState() const { return prevState; };

	/*
	
	*/
	inline vector<float> GetPrevAction() const { return prevAction; };
	
	/*
		
	*/
	inline void SetPrevAction(vector<float> newAction) { prevAction = newAction; };
	
	/*
	
	*/
	inline float GetReward() const { return reward; };
	
	/*
	
	*/
	inline void SetReward(float newReward) 
	{ 
		totalReward += newReward;
		reward = newReward; 
	};

	/*
	  
	*/
	inline float GetTotalReward() const { return totalReward; };
	
	/* 
	
	*/
	inline bool IsTerminal() const { return isTerminal; };
	
	/*
	
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
	vector<float> prevAction; 

	/* The reward previously earned in the environment. */
	float reward;

	/* The total rewards earned. */
	float totalReward;

	/* Is the current state of the environment a terminal state. */
	bool isTerminal;
};
