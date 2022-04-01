#pragma once

#include "../SAC/SACAgent.h"

class AgentTest
{
public:
	AgentTest();

	void Train();
	void Test();

private:
	SACAgent* agent;
};