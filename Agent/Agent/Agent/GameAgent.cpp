#include "GameAgent.h"

GameAgent::GameAgent(shared_ptr<Environment> env, NetworkParameters& params)
{
	// Initial Environment
	environment = env;

	// Initial Agent
	AgentFactory factory;
	agent = factory.GetNetworkAgent(params);
}

vector<float> GameAgent::PredictAction()
{
	return agent->PredictAction(environment->GetCurrState().ToTensor());
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

