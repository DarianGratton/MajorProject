#include "ACERAgent.h"

ACERAgent::ACERAgent(unsigned int lr, 
	unsigned int nActions, unsigned int nPossibleActions,
	int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims, 
	unsigned int memSize, unsigned int batchSize) :
	batchSize(batchSize)
{
	actorCritic = ActorCriticNetwork(lr, nActions, inputDims, hiddenLayerDims, actionLayerDims);
	memory = unique_ptr<ACERReplayMemory>(new ACERReplayMemory(memSize, inputDims, nActions, nPossibleActions));
}

ACERAgent::~ACERAgent()
{
	
}

void ACERAgent::UpdateMemory(
	State state,
	vector<float> actions,
	float reward,
	State newState,
	bool terminal,
	vector<float> actionProbabilities)
{
	memory->StoreStateTransition(state, actions, reward, newState, terminal, actionProbabilities);
}

void ACERAgent::Learn(bool onPolicy)
{
	// Reset gradients and initialize models parameters
	auto actorCriticClone = actorCritic->CustomClone();
	actorCriticClone->zero_grad();

	if (!onPolicy)
		// Sample the trajectory from the replay memory
		bool temp;
	else
		// Otherwise just use current state
		bool temp;

	// Loop through trajectories
	
	// In Loop: Compute policy and values (Appears to be calculated from the forward)

	// In Loop: Take step in environment
	if (onPolicy)
	{

	}

	// In Loop: Importance sampling weights ? (rho) ? ?(?|s_i) / µ(?|s_i); 1 for on-policy
	 
}

void ACERAgent::SaveModel()
{
}

void ACERAgent::LoadModel()
{
}
