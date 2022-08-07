// Environment.cpp

#include "Environment.h"

Environment::Environment(State initState) : initState(initState)
{
	Reset();
}

void Environment::Update(std::vector<float> newAction, float newReward, State newState, bool terminal)
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
	prevState = State();
	currState = initState;
	action.clear();
	reward = 0;
	totalReward = reward;
	isTerminal = false;
}
