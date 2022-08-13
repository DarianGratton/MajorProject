#include "Agent.h"

namespace GameAgent
{ 

Agent::Agent(std::shared_ptr<Environment> env, GameAgent::Networks::NetworkParameters& params)
{
	// Initialize Environment
	environment = env;

	// Initialize Agent
	AgentFactory factory;
	agent = factory.GetNetworkAgent(params);

	// Initialize Storage
	storage = std::make_unique<UtilityStorage>();
}

std::vector<float> Agent::PredictAction()
{
	return agent->PredictAction(environment->GetCurrState().ToTensor());
}

void Agent::Train()
{
	agent->Train(*environment);
}

void Agent::SaveAgent()
{
	agent->SaveModel();
}

void Agent::LoadAgent()
{
	agent->LoadModel();
}

void Agent::SaveEnvToStorage()
{
}

void Agent::ClearStorage()
{
	storage->Clear();
}

} /* namespace GameAgent */
