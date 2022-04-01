#include "SACAgent.h"

#include <typeinfo>
#include <iostream>
#include <ostream>
#include <istream>

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
	if (memory->GetCurrentMemsize() < batchSize)
		return;

	// Sample buffer
	ReplayMemory::MemorySample memorySample = memory->SampleMemory(batchSize);

	cout << "Running Learn: " << endl;
	try 
	{
		// Calculate the value of the states and new states based on the
		// value and target value networks
		torch::Tensor stateValue = value.get()->Forward(memorySample.states).view(-1);
		torch::Tensor stateTargetValue = targetValue.get()->Forward(memorySample.newStates).view(-1);
		stateTargetValue *= (~memorySample.terminals.clone()); // Dumb way of changing all values that are true to 0

		// Get actions and log probabilites for the states according to the 
		// new policy 
		pair<torch::Tensor, torch::Tensor> actionProb = policy.get()->CalculateActionProb(memorySample.states, false);
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

		// Policy loss
		torch::Tensor policyLoss = logProb - criticValue;
		policyLoss = torch::mean(policyLoss);
		policy.get()->GetOptimizer()->zero_grad();
		policyLoss.backward({}, true);
		policy.get()->GetOptimizer()->step();

		// Critic loss
		critic1.get()->GetOptimizer()->zero_grad();
		critic2.get()->GetOptimizer()->zero_grad();
		torch::Tensor qHat = rewardScale * memorySample.rewards + gamma * stateTargetValue;
		torch::Tensor oldPolicy1 = critic1.get()->Forward(memorySample.states, memorySample.actions).view(-1);
		torch::Tensor oldPolicy2 = critic2.get()->Forward(memorySample.states, memorySample.actions).view(-1);
		torch::Tensor critic1Loss = 0.5f * torch::nn::functional::mse_loss(oldPolicy1, qHat);
		torch::Tensor critic2Loss = 0.5f * torch::nn::functional::mse_loss(oldPolicy2, qHat);
	
		torch::Tensor criticLoss = critic1Loss + critic2Loss;
		criticLoss.backward();
		critic1.get()->GetOptimizer()->step();
		critic2.get()->GetOptimizer()->step();
	}
	catch (const c10::Error& e)
	{
		std::cout << "SACAgent::Learn Error: " << e.what() << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "SACAgent::Learn Error: " << e.what() << std::endl;
		exit(0);
	}

	// Update network parameters
	UpdateNetworkParameters(tau);
}

void SACAgent::SaveModel()
{
	// PolicyNetwork Checkpoint
	policy.get()->GetCheckpoint().str(string());
	torch::save(policy, policy.get()->GetCheckpoint());
	SaveToFile("Policy.txt", policy.get()->GetCheckpoint());

	// CriticNetwork Checkpoint
	critic1.get()->GetCheckpoint().str(string());
	torch::save(critic1, critic1.get()->GetCheckpoint());
	SaveToFile("Critic1.txt", critic1.get()->GetCheckpoint());

	critic2.get()->GetCheckpoint().str(string());
	torch::save(critic2, critic2.get()->GetCheckpoint());
	SaveToFile("Critic2.txt", critic2.get()->GetCheckpoint());

	// ValueNetwork Checkpoint
	value.get()->GetCheckpoint().str(string());
	torch::save(value, value.get()->GetCheckpoint());
	SaveToFile("Value.txt", value.get()->GetCheckpoint());

	targetValue.get()->GetCheckpoint().str(string());
	torch::save(targetValue, targetValue.get()->GetCheckpoint());
	SaveToFile("TargetValue.txt", targetValue.get()->GetCheckpoint());

	// Load Replay Memory
	memory.get()->SaveMemory();
}

void SACAgent::LoadModel()
{
	// PolicyNetwork Load Checkpoint
	LoadFromFile("Policy.txt", policy.get()->GetCheckpoint());
	torch::load(policy, policy.get()->GetCheckpoint());

	// CriticNetwork Load Checkpoint
	LoadFromFile("Critic1.txt", critic1.get()->GetCheckpoint());
	torch::load(critic1, critic1.get()->GetCheckpoint());
	LoadFromFile("Critic2.txt", critic2.get()->GetCheckpoint());
	torch::load(critic2, critic2.get()->GetCheckpoint());

	// ValueNetwork Load Checkpoint
	LoadFromFile("Value.txt", value.get()->GetCheckpoint());
	torch::load(value, value.get()->GetCheckpoint());
	LoadFromFile("TargetValue.txt", targetValue.get()->GetCheckpoint());
	torch::load(targetValue, targetValue.get()->GetCheckpoint());

	// Load Replay Memory
	// memory.get()->LoadMemory();
}

void SACAgent::SaveToFile(string filename, stringstream& checkpoint)
{
	// Save buffer
	ofstream outFile;
	outFile.open(filename, ios::binary | ios::trunc);
	outFile << checkpoint.rdbuf();
	outFile.close();
}

void SACAgent::LoadFromFile(string filename, stringstream& checkpoint)
{
	// Clear the stream
	checkpoint.str(string());

	// Load buffer
	ifstream inFile;
	inFile.open(filename, ios::binary);
	checkpoint << inFile.rdbuf();
	inFile.close();
}
