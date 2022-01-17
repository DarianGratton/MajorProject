#include "SACAgent.h"

SACAgent::SACAgent(float lr1, float lr2, 
	unsigned int nActions, unsigned int maxActions,
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims,
	unsigned int memSize, float gamma, float tau,
	unsigned int batchSize, int rewardScale) :
	numActions(nActions), gamma(gamma), 
	tau(tau), batchSize(batchSize), rewardScale(rewardScale)
{
	// To add checkpoint stuff
	memory		= unique_ptr<ReplayMemory>(new ReplayMemory(memSize, nActions));
	policy		= unique_ptr<PolicyNetwork>(new PolicyNetwork(lr1, nActions, maxActions, inputDims, layer1Dims, layer2Dims));
	critic1		= unique_ptr<CriticNetwork>(new CriticNetwork(lr2, nActions, inputDims, layer1Dims, layer2Dims));
	critic2		= unique_ptr<CriticNetwork>(new CriticNetwork(lr2, nActions, inputDims, layer1Dims, layer2Dims));
	value		= unique_ptr<ValueNetwork>(new ValueNetwork(lr2, inputDims, layer1Dims, layer2Dims));
	targetValue = unique_ptr<ValueNetwork>(new ValueNetwork(lr2, inputDims, layer1Dims, layer2Dims));
}

unsigned int SACAgent::ChooseAction(State observation)
{
	torch::Tensor state = observation.ToTensor();
	pair<torch::Tensor, torch::Tensor> actions = policy->CalculateActionProb(state, false);

	return *(actions.first.cpu().detach().data<int>());
}

void SACAgent::UpdateMemory()
{

}

void SACAgent::UpdateNetworkParameters()
{

}

void SACAgent::Learn()
{

}

void SACAgent::SaveModel()
{

}

void SACAgent::LoadModel()
{

}

