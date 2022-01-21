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

void SACAgent::UpdateMemory(
	list<State> state,
	list<unsigned int> action,
	long reward,
	list<State> newState,
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

