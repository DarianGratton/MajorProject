#include "GameAgent.h"

GameAgent::GameAgent(State initState, NetworkParameters& params)
{
	// Initial Environment
	environment = make_shared<Environment>(initState);

	// Initial Agent
	AgentFactory factory;
	agent = factory.GetNetworkAgent(params);
}

GameAgent::~GameAgent()
{
	
}

vector<float> GameAgent::PredictAction(State state)
{
	return agent->PredictAction(state.ToTensor());
}

void GameAgent::Train()
{
	agent->Train(*environment);
}

void GameAgent::SaveAgent()
{
	agent->SaveModel();
}

void GameAgent::LoadAgent()
{
	agent->LoadModel();
}

