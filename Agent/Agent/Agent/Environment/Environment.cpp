// Environment.cpp

#include "Environment.h"

Environment::Environment(State initState) : initState(initState)
{
	Reset();
}

void Environment::Reset()
{
	currState = initState;
	reward = 0;
	totalReward = reward;
	isTerminal = false;
}
