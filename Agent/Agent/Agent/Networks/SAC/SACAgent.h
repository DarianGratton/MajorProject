#pragma once

#include "ReplayMemory.h"
#include "PolicyNetwork.h"
#include "CriticNetwork.h"
#include "ValueNetwork.h"

#include "../../Environment/State.h"

#include <torch/torch.h>

#include <memory>

using namespace std;

// TODO: Implement functionality for a basic SACAgent
class SACAgent 
{
public:

	SACAgent(float lr1, float lr2, 
		unsigned int nActions, unsigned int maxActions,
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims,
		unsigned int memSize, float gamma, float tau,
		unsigned int batchSize, int rewardScale);

	// Subject to change
	unsigned int ChooseAction(State observation);
	void UpdateMemory();
	void UpdateNetworkParameters();
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
	unique_ptr<ReplayMemory>  memory;
	unique_ptr<PolicyNetwork> policy;
	unique_ptr<CriticNetwork> critic1;
	unique_ptr<CriticNetwork> critic2;
	unique_ptr<ValueNetwork>  value;
	unique_ptr<ValueNetwork>  targetValue;
};
