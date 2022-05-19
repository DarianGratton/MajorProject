#include "ACERAgent.h"

ACERAgent::ACERAgent(unsigned int lr, 
	unsigned int nActions, unsigned int nPossibleActions,
	int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims, 
	unsigned int memSize, unsigned int batchSize) :
	batchSize(batchSize)
{
	actorCritic = ActorCriticNetwork(lr, nActions, inputDims, hiddenLayerDims, actionLayerDims);
	memory = unique_ptr<ACERReplayMemory>(new ACERReplayMemory(memSize, 10, inputDims, nActions, nPossibleActions));
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

void ACERAgent::Learn(Trajectory trajectory, bool onPolicy)
{
	// Reset gradients and initialize models parameters
	auto actorCriticClone = actorCritic->CustomClone();
	actorCriticClone->zero_grad();

	// 
	torch::Tensor newStates = trajectory.newStates.index({trajectory.newStates.size(0) - 1});
	auto predictActions = actorCriticClone->Forward(newStates);
	torch::Tensor Qret = (predictActions.first * predictActions.second).data().sum(-1).unsqueeze(-1);

	// Loop through trajectories
	for (int i = 0; i < trajectory.states.size(0); ++i)
	{
		// In Loop: Compute policy and values (Appears to be calculated from the forward)


		// In Loop: Importance sampling weights ? (rho) ? ?(?|s_i) / µ(?|s_i); 1 for on-policy
	}
}

void ACERAgent::SaveModel()
{
}

void ACERAgent::LoadModel()
{
}
