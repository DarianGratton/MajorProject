#pragma once

#include "../ACER/ACERAgent.h"
#include "../../Environment/Environment.h"
#include "../../Agent.h"

#include <memory>

class AgentTest
{
public:
	AgentTest(bool isContinous = false, bool recordOutput = false);

	void Train();
	void Test();

private:
	/* Agent pointer. */
	std::unique_ptr<GameAgent::Agent> gameAgent;
	
	/* Environment pointer. */
	std::shared_ptr<GameAgent::Environment> environment;

	/* Action space of Environment used. */
	bool isContinous;

	/* If true results will be recorded in a csv. */
	bool recordOutput;

	unsigned int batchSize;
};