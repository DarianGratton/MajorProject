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
	torch::Tensor stateValue = value->get()->Forward(memorySample.states).view(-1);
	torch::Tensor stateTargetValue = targetValue->get()->Forward(memorySample.newStates).view(-1);
	stateTargetValue[memorySample.terminals] = 0.0f;

	// Get actions and log probabilites for the states according to the 
	// new policy 
	pair<torch::Tensor, torch::Tensor> actionProb = policy->get()->CalculateActionProb(memorySample.states, false);
	torch::Tensor logProb = actionProb.second.view(-1);
	torch::Tensor newPolicy1 = critic1->get()->Forward(memorySample.states, actionProb.first);
	torch::Tensor newPolicy2 = critic2->get()->Forward(memorySample.states, actionProb.first);
	torch::Tensor criticValue = torch::min(newPolicy1, newPolicy2);
	criticValue = criticValue.view(-1);
	
	// Value loss
	value->get()->GetOptimizer()->zero_grad();
	torch::Tensor valueTarget = criticValue - actionProb.second;
	torch::Tensor valueLoss = 0.5f * torch::nn::functional::mse_loss(stateValue, valueTarget);
	valueLoss.backward({}, true);
	value->get()->GetOptimizer()->step();

	// Get actions and log probabilites for the states according to the 
	// new policy 
	actionProb = policy->get()->CalculateActionProb(memorySample.states, true);
	logProb = actionProb.second.view(-1);
	newPolicy1 = critic1->get()->Forward(memorySample.states, actionProb.first);
	newPolicy2 = critic2->get()->Forward(memorySample.states, actionProb.first);
	criticValue = torch::minimum(newPolicy1, newPolicy2);
	criticValue = criticValue.view(-1);

	// Policy loss
	torch::Tensor policyLoss = actionProb.second - criticValue;
	policyLoss = torch::mean(policyLoss);
	policy->get()->GetOptimizer()->zero_grad();
	policyLoss.backward({}, true);
	policy->get()->GetOptimizer()->step();

	// Critic loss
	critic1->get()->GetOptimizer()->zero_grad();
	critic2->get()->GetOptimizer()->zero_grad();
	torch::Tensor qHat = rewardScale * memorySample.rewards + gamma * stateTargetValue;
	torch::Tensor oldPolicy1 = critic1->get()->Forward(memorySample.states, memorySample.actions).view(-1);
	torch::Tensor oldPolicy2 = critic2->get()->Forward(memorySample.states, memorySample.actions).view(-1);
	torch::Tensor critic1Loss = 0.5 * torch::nn::functional::mse_loss(oldPolicy1, qHat);
	torch::Tensor critic2Loss = 0.5 * torch::nn::functional::mse_loss(oldPolicy2, qHat);
	torch::Tensor criticLoss = critic1Loss + critic2Loss;
	criticLoss.backward();
	critic1->get()->GetOptimizer()->step();
	critic2->get()->GetOptimizer()->step();

	// Update network parameters
	UpdateNetworkParameters(tau);
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

