// Environment.cpp

#include "Environment.h"

Environment::Environment(State initState) : initState(initState)
{
	Reset();
}

void Environment::Update(vector<float> newAction, float newReward, State newState, bool terminal)
{
	// Update action
	action = newAction;

	// Update reward
	totalReward += newReward;
	reward = newReward;

	// Update state
	prevState = currState;
	currState = newState;

	// Update terminal
	isTerminal = terminal;
}

void Environment::Reset()
{
	currState = initState;
	reward = 0;
	totalReward = reward;
	isTerminal = false;
}
