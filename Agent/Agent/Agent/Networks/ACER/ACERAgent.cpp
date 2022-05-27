#include "ACERAgent.h"

ACERAgent::ACERAgent(unsigned int lr, 
	unsigned int nActions, unsigned int nPossibleActions,
	int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims, 
	unsigned int memSize, unsigned int batchSize,
	float biaWeight, float gamma, int traceMax) :
	batchSize(batchSize), biasWeight(biasWeight), 
	gamma(gamma), traceMax(traceMax)
{
	// TODO: Need to call share_memory() or something, but C++ API doesn't have it
	// should figure out how to implement when doing multithreading
	actorCritic = ActorCriticNetwork(lr, nActions, inputDims, hiddenLayerDims, actionLayerDims);
	averageActorCritic = actorCritic->CustomClone();
	averageActorCritic->zero_grad();
	
	optimizer = new torch::optim::Adam(actorCritic->parameters(), lr);
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

void ACERAgent::Learn(std::vector<Trajectory> trajectories, bool onPolicy)
{
	// Reset gradients and initialize models parameters
	// TODO: Replace 0.99 with a variable
	auto actorCriticClone = actorCritic->CustomClone();

	int index = 0;
	std::vector<torch::Tensor> actorCloneParameters = actorCriticClone ->parameters();
	for (torch::Tensor actorParameters : actorCritic->parameters())
	{
		actorCloneParameters[index].data().copy_(0.99f * actorCloneParameters[index].data() * (1 - 0.99f));
		index++;
	}

	actorCriticClone->zero_grad();

	// Calculate Retrace: Qret
	Trajectory trajectory = trajectories[trajectories.size() - 1];
	torch::Tensor newStates = trajectory.newStates.index({ trajectory.newStates.size(0) - 1 });
	auto predictedActions = actorCriticClone->Forward(newStates);
	torch::Tensor Qret = (predictedActions.first * predictedActions.second).data().sum(-1).unsqueeze(-1);

	// Loop through trajectories
	for (int i = trajectories.size(); i >= 0; --i)
	{
		// Calculate state-action values 
		trajectory = trajectories[i];
		predictedActions = actorCriticClone->Forward(trajectory.states);
		auto avgPredictedActions = averageActorCritic->Forward(trajectory.states);

		// ρi ← f(ai | φθ′(xi)) / μ(ai | xi)
		torch::Tensor importanceWeights = predictedActions.first.data() / trajectory.policy;

		// V ← the expection of Q under π
		torch::Tensor value = (predictedActions.first * predictedActions.second).data().sum(-1).unsqueeze(-1) * (trajectory.terminals - 1);

		// Qret ← r_i + γQret (If terminal is true, Qret equals 0)
		Qret = trajectory.rewards + gamma * (Qret * (1.0f - trajectory.terminals));

		// Advantage A ← Qret - V(s_i; θ)
		torch::Tensor QretAdvantage = Qret - value;

		// Actor Loss
		// g ← min(c, ρ_a_i)∙∇θ∙log(π(a_i|s_i; θ))∙A
		torch::Tensor actorLoss =
			importanceWeights.gather(-1, trajectory.actions.data()).clamp(0, traceMax)
			* biasWeight
			* predictedActions.first.gather(-1, trajectory.actions).log()
			* QretAdvantage;

		// g = g + Σ_a [1 - c/ρ_a]_+∙π(a|s_i; θ)∙∇θ∙log(π(a|s_i; θ))∙(Q(s_i, a; θ) - V(s_i; θ)
		actorLoss +=
			((1 - traceMax / importanceWeights).clamp(0)
				* predictedActions.first.data()
				* biasWeight
				* predictedActions.first.log()
				* (predictedActions.second.gather(-1, trajectory.actions).data() - value)).sum(-1).unsqueeze(-1);

		// Calculate gradients
		std::vector<torch::Tensor> actorGradients = torch::autograd::grad({ actorLoss.mean() }, { predictedActions.first }, {}, true);

		// Trust Region update
		torch::Tensor trustRegionGrad = TrustRegion(actorGradients, predictedActions.first, avgPredictedActions.first.data());
		predictedActions.first.backward(trustRegionGrad, true);

		// Critic
		// dθ ← (Qret - Q(s_i, a_i; θ)) ^ 2
		torch::Tensor criticLoss = (predictedActions.second.gather(-1, predictedActions.second) - Qret).pow(2);

		// dθ ← dθ - ∇θ
		criticLoss.mean().backward({}, true);

		// Entropy
		// TODO: 1e-3 should be a variable called entropy regularization or something
		// TODO: Figure out formula this even follows
		torch::Tensor entropyLoss = 1e-3 * (predictedActions.first * predictedActions.first.log()).sum(-1);
		entropyLoss.mean().backward({}, true);

		// Update retrace
		// Qret ← ρ¯_a_i∙(Qret - Q(s_i, a_i; θ)) + V(s_i; θ)
		Qret =
			importanceWeights.gather(-1, predictedActions.second.data()).clamp({}, 1)
			* (Qret - predictedActions.second.gather(-1, predictedActions.second).data())
			+ value;
	}

	// Update networks
	index = 0;
	std::vector<torch::Tensor> actorParameters = actorCritic->parameters();
	for (torch::Tensor cloneActorParameter : actorCriticClone->parameters())
	{
		actorParameters[index].mutable_grad() = cloneActorParameter.grad().clone();
		index++;
	}

	index = 0;
	std::vector<torch::Tensor> avgActorParameters = averageActorCritic->parameters();
	for (torch::Tensor actorParameters : actorCritic->parameters())
	{
		avgActorParameters[index].data().copy_(0.99f * avgActorParameters[index].data() + (1 - 0.99f) * actorParameters.data());
		index++;
	}

	// Optimizer
	optimizer->step();
}

void ACERAgent::SaveModel()
{
}

void ACERAgent::LoadModel()
{
}

torch::Tensor ACERAgent::TrustRegion(std::vector<torch::Tensor> gradients,
	torch::Tensor policy, torch::Tensor averagePolicy)
{
	torch::Tensor kullbackLeibler = -((averagePolicy.log() - policy.log()) * averagePolicy).sum(-1);
	std::vector<torch::Tensor> kullbackLeiblerGrad = torch::autograd::grad({ kullbackLeibler.mean() }, { policy }, {}, true);

	// TODO: Unknown why he uses the for loop?
	// TODO: Test if gradients[0] is accessing the correct thing
	torch::Tensor scale = gradients[0].mul(kullbackLeiblerGrad[0]).sum(-1).unsqueeze(-1) - 1;
	scale = scale.div(gradients[0].mul(gradients[0].sum(-1).unsqueeze(-1))).clamp(0);
	torch::Tensor updatedGradients = gradients[0] - scale * kullbackLeiblerGrad[0];

	return updatedGradients;
}
