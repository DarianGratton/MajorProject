// Environment.cpp

#include "Environment.h"

namespace GameAgent
{

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
	rewardHistory.push_back(newReward);

	// Update state
	prevState = currState;
	currState = newState;

	// Update terminal
	isTerminal = terminal;

	// Update steps
	steps++;
}

void Environment::Reset()
{
	prevState = State();
	currState = initState;
	action.clear();
	reward = 0;
	totalReward = reward;
	rewardHistory.clear();
	isTerminal = false;
	steps = 0;
}

} /* namespace GameAgent */
