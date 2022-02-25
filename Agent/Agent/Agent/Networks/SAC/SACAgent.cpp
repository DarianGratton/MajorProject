#include "SACAgent.h"

#include <typeinfo>
#include <iostream>

SACAgent::SACAgent(float lr1, float lr2, 
	unsigned int nActions, unsigned int maxActions,
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims,
	unsigned int memSize, float gamma, float tau,
	unsigned int batchSize, int rewardScale) :
	numActions(nActions), gamma(gamma), 
	tau(tau), batchSize(batchSize), rewardScale(rewardScale)
{
	// To add checkpoint stuff
	memory				 = unique_ptr<ReplayMemory>(new ReplayMemory(memSize, inputDims, nActions));
	policy				 = PolicyNetwork(lr1, nActions, maxActions, inputDims, layer1Dims, layer2Dims);
	critic1				 = CriticNetwork(lr2, nActions, inputDims, layer1Dims, layer2Dims);
	critic2				 = CriticNetwork(lr2, nActions, inputDims, layer1Dims, layer2Dims);
	value				 = ValueNetwork(lr2, inputDims, layer1Dims, layer2Dims);
	targetValue			 = ValueNetwork(lr2, inputDims, layer1Dims, layer2Dims);

	// Initialize network parameters
	UpdateNetworkParameters(1);
}

torch::Tensor SACAgent::ChooseAction(State& observation)
{
	torch::Tensor state = observation.ToTensor();
	pair<torch::Tensor, torch::Tensor> actions = policy.get()->CalculateActionProb(state, false);

	// Test
	// pair<torch::Tensor, torch::Tensor> actions =
	//     make_pair<torch::Tensor, torch::Tensor>(torch::tensor({ {-0.1143, 0.1232} }), torch::Tensor());
	// cout << actions.first.cpu().detach().data()[0] << endl;
	// cout << typeid(actions.first.cpu().detach().data()[0]).name() << endl;

	try 
	{
		return actions.first.cpu().detach().data()[0];
	}
	catch (const c10::Error& e)
	{
		std::cerr << "SACAgent::ChooseAction error: " << e.msg() << std::endl;
		return torch::Tensor();
	}
}

void SACAgent::UpdateMemory(
	State state,
	vector<float> actions,
	float reward,
	State newState,
	bool terminal)
{
	memory->StoreStateTransition(state, actions, reward, newState, terminal);
}

// TODO: This is broken
void SACAgent::UpdateNetworkParameters(float T /* tau */)
{
	try
	{
		// Get parameters and convert them into maps
		vector<pair<string, torch::Tensor>> targetValueParams = targetValue.get()->named_parameters().pairs();
		vector<pair<string, torch::Tensor>> valueParams = value.get()->named_parameters().pairs();

		map<string, torch::Tensor> targetValueParamsMap(targetValueParams.begin(), targetValueParams.end());
		map<string, torch::Tensor> valueParamsMap(valueParams.begin(), valueParams.end());

		// Update Network Parameters
		for (pair<string, torch::Tensor> name : valueParamsMap)
		{
			targetValue.get()->named_parameters()[name.first].data() = 
				T * valueParamsMap.at(name.first).clone() +
				(1 - T) * targetValueParamsMap.at(name.first).clone();
		}
	}
	catch (const c10::Error& e)
	{
		std::cout << "UpdateNetworkParameters Error: " << e.msg() << std::endl;
	}
}

void SACAgent::Learn()
{
	// See if there is enough memory to sample
	// if (memory->GetCurrentMemsize() < batchSize)
	//	return;

	// Sample buffer
	ReplayMemory::MemorySample memorySample = memory->SampleMemory(batchSize);

	memorySample.rewards = torch::tensor({ 1, 1, 1, 1, 1 }, torch::kF32);
	memorySample.terminals = torch::tensor({ false, false, true, false, false }, torch::kBool);
	memorySample.newStates = torch::tensor({
		{-8.9976e-04, -5.2816e-03, 9.9888e-01, -4.7340e-02, -6.3622e-02},
		{-5.7198e-04, -1.8270e-02, 9.9895e-01, -4.5812e-02, -6.3999e-03},
		{-8.9976e-04, -5.2816e-03, 9.9888e-01, -4.7340e-02, -6.3622e-02},
		{-8.1262e-04, -1.4584e-02, 9.9893e-01, -4.6291e-02, -2.9054e-02},
		{-8.9976e-04, -5.2816e-03, 9.9888e-01, -4.7340e-02, -6.3622e-02} },
		torch::kF32);
	memorySample.states = torch::tensor({
		{-8.1262e-04, -1.4584e-02, 9.9893e-01, -4.6291e-02, -2.9054e-02},
		{-2.7052e-04, -1.1070e-02, 9.9895e-01, -4.5707e-02, -6.5699e-03},
		{-8.1262e-04, -1.4584e-02, 9.9893e-01, -4.6291e-02, -2.9054e-02},
		{-5.7198e-04, -1.8270e-02, 9.9895e-01, -4.5812e-02, -6.3999e-03},
		{-8.1262e-04, -1.4584e-02, 9.9893e-01, -4.6291e-02, -2.9054e-02} },
		torch::kF32);
	memorySample.actions = torch::tensor({ {0.0557}, {-0.0675}, {0.0557}, {0.0138}, {0.0557} }, torch::kF32);

	// Calculate the value of the states and new states based on the
	// value and target value networks
	torch::Tensor stateValue = value.get()->Forward(memorySample.states).view(-1);
	torch::Tensor stateTargetValue = targetValue.get()->Forward(memorySample.newStates).view(-1);
	stateTargetValue *= (~memorySample.terminals.clone()); // Dumb way of changing all values that are true to 0

	// ^ Tested       Untested (Below) //

	// Get actions and log probabilites for the states according to the 
	// new policy 
	pair<torch::Tensor, torch::Tensor> actionProb = policy.get()->CalculateActionProb(memorySample.states, false);
	
	cout << actionProb.first << endl;
	cout << actionProb.second << endl;

	exit(0);
	
	torch::Tensor logProb = actionProb.second.view(-1);
	torch::Tensor newPolicy1 = critic1.get()->Forward(memorySample.states, actionProb.first);
	torch::Tensor newPolicy2 = critic2.get()->Forward(memorySample.states, actionProb.first);

	torch::Tensor criticValue = torch::min(newPolicy1, newPolicy2);
	criticValue = criticValue.view(-1);
	
	// Value loss
	value.get()->GetOptimizer()->zero_grad();
	torch::Tensor valueTarget = criticValue - logProb;
	torch::Tensor valueLoss = 0.5f * torch::nn::functional::mse_loss(stateValue, valueTarget);
	valueLoss.backward({}, true);
	value.get()->GetOptimizer()->step();

	// Get actions and log probabilites for the states according to the 
	// new policy 
	actionProb = policy.get()->CalculateActionProb(memorySample.states, true);
	logProb = actionProb.second.view(-1);
	newPolicy1 = critic1.get()->Forward(memorySample.states, actionProb.first);
	newPolicy2 = critic2.get()->Forward(memorySample.states, actionProb.first);
	criticValue = torch::min(newPolicy1, newPolicy2);
	criticValue = criticValue.view(-1);

	// cout << criticValue << endl;

	// Policy loss
	torch::Tensor policyLoss = logProb - criticValue;
	policyLoss = torch::mean(policyLoss);
	policy.get()->GetOptimizer()->zero_grad();
	policyLoss.backward({}, true);
	policy.get()->GetOptimizer()->step();

	// cout << policyLoss << endl;

	// Critic loss
	critic1.get()->GetOptimizer()->zero_grad();
	critic2.get()->GetOptimizer()->zero_grad();
	torch::Tensor qHat = rewardScale * memorySample.rewards + gamma * stateTargetValue;
	torch::Tensor oldPolicy1 = critic1.get()->Forward(memorySample.states, memorySample.actions).view(-1);
	torch::Tensor oldPolicy2 = critic2.get()->Forward(memorySample.states, memorySample.actions).view(-1);
	torch::Tensor critic1Loss = 0.5 * torch::nn::functional::mse_loss(oldPolicy1, qHat);
	torch::Tensor critic2Loss = 0.5 * torch::nn::functional::mse_loss(oldPolicy2, qHat);
	torch::Tensor criticLoss = critic1Loss + critic2Loss;

	// cout << criticLoss << endl;

	criticLoss.backward();
	critic1.get()->GetOptimizer()->step();
	critic2.get()->GetOptimizer()->step();

	// Update network parameters
	UpdateNetworkParameters(tau);

}

void SACAgent::SaveModel()
{
	// PolicyNetwork Checkpoint
	policy.get()->GetCheckpoint().str(string());
	torch::save(policy, policy.get()->GetCheckpoint());

	// CriticNetwork Checkpoint
	critic1.get()->GetCheckpoint().str(string());
	torch::save(critic1, critic1.get()->GetCheckpoint());

	critic2.get()->GetCheckpoint().str(string());
	torch::save(critic2, critic2.get()->GetCheckpoint());

	// ValueNetwork Checkpoint
	value.get()->GetCheckpoint().str(string());
	torch::save(value, value.get()->GetCheckpoint());

	targetValue.get()->GetCheckpoint().str(string());
	torch::save(targetValue, targetValue.get()->GetCheckpoint());
}

void SACAgent::LoadModel()
{
	// PolicyNetwork Load Checkpoint
	torch::load(policy, policy.get()->GetCheckpoint());

	// CriticNetwork Load Checkpoint
	torch::load(critic1, critic1.get()->GetCheckpoint());
	torch::load(critic2, critic2.get()->GetCheckpoint());

	// ValueNetwork Load Checkpoint
	torch::load(value, value.get()->GetCheckpoint());
	torch::load(targetValue, targetValue.get()->GetCheckpoint());
}

