#pragma once

#include <torch/torch.h>

#include <vector>
#include <memory>

#include "Environment/Environment.h"
#include "Environment/State.h"
#include "Networks/NetworkAgent.h"
#include "Networks/NetworkParameters.h"
#include "Networks/AgentFactory.h"

using namespace std;

/* 
  GameAgent

	

  Author: Darian G.
*/
class GameAgent
{
public:

	/* */
	GameAgent(State initState, NetworkParameters& params);

	~GameAgent();

	vector<float> PredictAction(State state);

	void Train();

	void SaveAgent();

	void LoadAgent();

	inline shared_ptr<Environment> GetEnvironment() { return environment; }

private:
	shared_ptr<Environment> environment;
	shared_ptr<NetworkAgent> agent;
};