#pragma once

#include "ReplayMemory.h"
#include "PolicyNetwork.h"
#include "CriticNetwork.h"
#include "ValueNetwork.h"

#include "../../Environment/State.h"

#include <torch/torch.h>

#include <memory>
#include <sstream>

using namespace std;
using namespace RLGameAgent;

/// <summary>
/// 
/// </summary>
class SACAgent 
{
public:

	SACAgent(float lr1, float lr2, 
		unsigned int nActions, unsigned int maxActions,
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims,
		unsigned int memSize, float gamma, float tau,
		unsigned int batchSize, int rewardScale);

	// Subject to change
	// TODO: need copy constructor
	torch::Tensor ChooseAction(State& observation);
	
	void UpdateMemory(
		State state,
		vector<float> actions,
		float reward,
		State newState,
		bool terminal);

	void UpdateNetworkParameters(float T);
	void Learn();

	void SaveModel();
	void LoadModel();

private:
	// Reward scale is how we handle entropy
	// Batch size used for memory buffer
	// Tau the factor that we use to change the parameters of the network
	// Gamma
	// memSize memory size
	// Network Variables
	float gamma;
	float tau;
	float numActions;
	int rewardScale;
	unsigned int batchSize;

	// Memory and Network objects
	unique_ptr<ReplayMemory> memory;
	PolicyNetwork policy = nullptr;
	CriticNetwork critic1 = nullptr;
	CriticNetwork critic2 = nullptr;
	ValueNetwork value = nullptr;
	ValueNetwork targetValue = nullptr;

	// Probably shouldn't be here but they are helper functions for saving and loading the models
	void SaveToFile(string filename, stringstream& checkpoint);
	void LoadFromFile(string filename, stringstream& checkpoint);
};
