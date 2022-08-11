#include "Agent.h"

namespace GameAgent
{ 

Agent::Agent(std::shared_ptr<Environment> env, GameAgent::Networks::NetworkParameters& params)
{
	// Initial Environment
	environment = env;

	// Initial Agent
	AgentFactory factory;
	agent = factory.GetNetworkAgent(params);
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

} /* namespace GameAgent */
