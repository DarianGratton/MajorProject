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
	memory		= unique_ptr<ReplayMemory>(new ReplayMemory(memSize, inputDims, nActions));
	policy		= new PolicyNetwork(lr1, nActions, maxActions, inputDims, layer1Dims, layer2Dims);
	critic1		= new CriticNetwork(lr2, nActions, inputDims, layer1Dims, layer2Dims);
	critic2		= new CriticNetwork(lr2, nActions, inputDims, layer1Dims, layer2Dims);
	value		= new ValueNetwork(lr2, inputDims, layer1Dims, layer2Dims);
	targetValue = new ValueNetwork(lr2, inputDims, layer1Dims, layer2Dims);

	// Initialize network parameters
	UpdateNetworkParameters(1);
}

unsigned int SACAgent::ChooseAction(State observation)
{
	torch::Tensor state = observation.ToTensor();
	pair<torch::Tensor, torch::Tensor> actions = policy->get()->CalculateActionProb(state, false);

	return *(actions.first.cpu().detach().data<int>());
}

void SACAgent::UpdateMemory(
	State state,
	float action,
	float reward,
	State newState,
	bool terminal)
{
	memory->StoreStateTransition(state, action, reward, newState, terminal);
}

void SACAgent::UpdateNetworkParameters(float T /* tau */)
{
	// Get parameters and convert them into maps
	vector<pair<string, torch::Tensor>> targetValueParams = targetValue->get()->named_parameters().pairs();
	vector<pair<string, torch::Tensor>> valueParams = value->get()->named_parameters().pairs();

	map<string, torch::Tensor> targetValueParamsMap((targetValueParams.begin()), targetValueParams.end());
	map<string, torch::Tensor> valueParamsMap((valueParams.begin()), valueParams.end());;

	// Update Network Parameters
	// TODO: Is untested, need to test
	int i = 0;
	for (pair<string, torch::Tensor> name : valueParamsMap)
	{
		targetValue->get()->parameters()[i].data() = T * valueParamsMap.at(name.first).clone() *
											    (1 - T) * targetValueParamsMap.at(name.first).clone();
		i++;
	}
}

void SACAgent::Learn()
{
	// See if there is enough memory to sample
	if (memory->GetCurrentMemsize() < batchSize)
		return;

	// Sample buffer
	ReplayMemory::MemorySample memorySample = memory->SampleMemory(batchSize);

	// Calculate the value of the states and new states based on the
	// value and target value networks
	// Notes:
	// - .view(-1) just means it collaspe along the batch dim due to loss
	// - should test .view(-1) to see what it is doing as well as to see how to recreate it if needed 
	// - where new state is terminal we want to set the new state to 0.0\


	// Get actions and log probabilites for the states according to the 
	// new policy 
	// Notes:
	// - T.min() improves stability of learning
	// - Do this for value network and the policy network
	

	// Calculate loss and back propagate
	// Notes:
	// - Retain graph as pytorch will by default discard the graph calculation,
	//   this will help us keep track of coupling between losses
	// - Do this for value network and the policy network
	
	// Critic loss

	// Update network parameters
}

void SACAgent::SaveModel()
{
	// PolicyNetwork Checkpoint
	policy->get()->GetCheckpoint().str(string());
	torch::save(*policy, policy->get()->GetCheckpoint());

	// CriticNetwork Checkpoint
	critic1->get()->GetCheckpoint().str(string());
	torch::save(*critic1, critic1->get()->GetCheckpoint());

	critic2->get()->GetCheckpoint().str(string());
	torch::save(*critic2, critic2->get()->GetCheckpoint());

	// ValueNetwork Checkpoint
	value->get()->GetCheckpoint().str(string());
	torch::save(*value, value->get()->GetCheckpoint());

	targetValue->get()->GetCheckpoint().str(string());
	torch::save(*targetValue, targetValue->get()->GetCheckpoint());
}

void SACAgent::LoadModel()
{
	// PolicyNetwork Load Checkpoint
	torch::load(*policy, policy->get()->GetCheckpoint());

	// CriticNetwork Load Checkpoint
	torch::load(*critic1, critic1->get()->GetCheckpoint());
	torch::load(*critic2, critic2->get()->GetCheckpoint());

	// ValueNetwork Load Checkpoint
	torch::load(*value, value->get()->GetCheckpoint());
	torch::load(*targetValue, targetValue->get()->GetCheckpoint());
}

