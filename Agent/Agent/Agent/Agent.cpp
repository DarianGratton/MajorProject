﻿#include "Agent.h"

#include <numeric>

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

void Agent::SaveUtility()
{
	// Calculate Utility
	std::vector<float> rewardHistory = environment->GetRewardHistory();
	float rewardCount = static_cast<float>(rewardHistory.size());

	if (rewardCount == 0)
	{
		std::cerr << "Agent::SaveUtility(): The RewardHistory of the environment is empty." << std::endl;
		return;
	}

	float utility = std::accumulate(rewardHistory.begin(), rewardHistory.end(), 0.0f) / rewardCount;
	 
	// Save to Storage
	storage->Save(environment->GetInitState(), utility);
}

std::vector<std::pair<State, float>> Agent::SearchUtilityStorage(State state)
{
	return storage->Search(state);
}

void Agent::ClearStorage()
{
	storage->Clear();
}

} /* namespace GameAgent */
