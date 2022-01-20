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
	policy		= new PolicyNetwork(lr1, nActions, maxActions, inputDims, layer1Dims, layer2Dims);
	critic1		= new CriticNetwork(lr2, nActions, inputDims, layer1Dims, layer2Dims);
	critic2		= new CriticNetwork(lr2, nActions, inputDims, layer1Dims, layer2Dims);
	value		= new ValueNetwork(lr2, inputDims, layer1Dims, layer2Dims);
	targetValue = new ValueNetwork(lr2, inputDims, layer1Dims, layer2Dims);
}

unsigned int SACAgent::ChooseAction(State observation)
{
	torch::Tensor state = observation.ToTensor();
	pair<torch::Tensor, torch::Tensor> actions = policy->get()->CalculateActionProb(state, false);

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
	// PolicyNetwork Checkpoint
	policy->get()->GetCheckpoint().clear();
	torch::save(*policy, policy->get()->GetCheckpoint());

	// CriticNetwork Checkpoint
	critic1->get()->GetCheckpoint().clear();
	torch::save(*critic1, critic1->get()->GetCheckpoint());

	critic2->get()->GetCheckpoint().clear();
	torch::save(*critic2, critic2->get()->GetCheckpoint());

	// ValueNetwork Checkpoint
	value->get()->GetCheckpoint().clear();
	torch::save(*value, value->get()->GetCheckpoint());

	targetValue->get()->GetCheckpoint().clear();
	torch::save(*targetValue, targetValue->get()->GetCheckpoint());
}

void SACAgent::LoadModel()
{

}

