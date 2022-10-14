#include "ACERAgent.h"

ACERAgent::ACERAgent(const GameAgent::Networks::ACERParameters& params) :
	maxEpisodeLength(params.maxEpisodeLength), batchSize(params.batchSize), 
	batchTrajectoryLength(params.batchTrajectoryLength), 
	actorLossWeight(params.actorLossWeight),
	gamma(params.gamma), truncationParameter(params.truncationParameter),
	trustRegionConstraint(params.trustRegionConstraint), 
	trustRegionDecay(params.trustRegionDecay)
{
	// Initialize Networks
	actorCritic = ActorCriticNetwork(params.learningRate, params.nPossibleActions, params.inputDims, params.nHiddenLayers, params.hiddenLayerDims, params.actionLayerDims);
	averageActorCritic = actorCritic->CleanClone();

	// Initialize Memory
	memory = std::unique_ptr<ACERReplayMemory>(new ACERReplayMemory(params.memSize, params.maxEpisodeLength, params.inputDims, params.nActions, params.nPossibleActions));
}

std::vector<float> ACERAgent::PredictAction(torch::Tensor state)
{
	// Predict Probabilites
	auto predictedAction = actorCritic->Forward(state);

	// Get Action
	// TODO: Modify for multiple actions (Out-of-scope)
	float action = predictedAction.first.multinomial(1).item<float>();

	return { action };
}

void ACERAgent::Train(const GameAgent::Environment::Transition& transition)
{
	UpdateMemory(transition.prevState, transition.action, 
				 transition.reward, transition.currState, transition.terminal);

	// Check if the agent is finished an episode in the environment
	if (transition.terminal || transition.currStep >= maxEpisodeLength)
	{
		// On-Policy
		Learn({ memory->GetPrevTrajectory() }, true);

		// Off-Policy
		if (memory->GetCurrentMemsize() >= batchSize)
		{
			std::vector<Trajectory> trajectories = memory->SampleMemory(batchSize, batchTrajectoryLength);
			Learn(trajectories, false);
		}
	}
}

void ACERAgent::UpdateMemory(
	GameAgent::State state,
	std::vector<float> actions,
	float reward,
	GameAgent::State newState,
	bool terminal)
{
	// Get Current Policy
	auto pred = actorCritic->Forward(state.ToTensor());
	std::vector<float> policy;
	for (int i = 0; i < pred.first.size(1); i++)
	{
		policy.push_back(pred.first[0][i].item<float>());
	}

	// Insert Transition into memory
	memory->StoreStateTransition(state, actions, reward, newState, terminal, policy);
}

void ACERAgent::SaveModel()
{
	// Save checkpoint
	actorCritic->GetCheckpoint().str(std::string());
	torch::save(actorCritic, actorCritic->GetCheckpoint());

	averageActorCritic->GetCheckpoint().str(std::string());
	torch::save(averageActorCritic, averageActorCritic->GetCheckpoint());

	// Save checkpoint to file
	std::ofstream outFile;
	outFile.open("ACERActorCritic.txt", std::ios::binary | std::ios::trunc);
	outFile << actorCritic->GetCheckpoint().rdbuf();
	outFile.close();

	outFile.open("ACERAvgActorCritic.txt", std::ios::binary | std::ios::trunc);
	outFile << averageActorCritic->GetCheckpoint().rdbuf();
	outFile.close();

	// Save Memory
	memory->Save();
}

void ACERAgent::LoadModel()
{
	// Clear the Checkpoint
	actorCritic->GetCheckpoint().str(std::string());
	averageActorCritic->GetCheckpoint().str(std::string());

	// Load Checkpoint from file
	std::ifstream inFile;
	inFile.open("ACERActorCritic.txt", std::ios::binary);
	actorCritic->GetCheckpoint() << inFile.rdbuf();
	inFile.close();

	inFile.open("ACERAvgActorCritic.txt", std::ios::binary);
	averageActorCritic->GetCheckpoint() << inFile.rdbuf();
	inFile.close();

	try 
	{
		// Load Checkpoint
		torch::load(actorCritic, actorCritic->GetCheckpoint());
		torch::load(averageActorCritic, averageActorCritic->GetCheckpoint());
	}
	catch (const c10::Error& e)
	{
		// File not found
	}

	// Load Memory
	memory->Load();
}

void ACERAgent::Learn(std::vector<Trajectory> trajectories, bool onPolicy)
{
	// Reset gradients and initialize models parameters
	auto newActorCritic = actorCritic->CleanClone();
	newActorCritic->CopyParametersFrom(*actorCritic);
	actorCritic->GetOptimizer()->zero_grad();

	// Get trajectory values
	// TODO: Refactor trajectory, currently confusing for anybody that doesn't know the code inside and out.
	Trajectory trajectory;
	if (onPolicy)
	{
		trajectory = Trajectory(1, trajectories[0].stateSize, trajectories[0].actions.size(1), trajectories[0].policy.size(1));
		trajectory.newStates[0] = trajectories[0].newStates[0];
	}
	else
	{
		trajectory = trajectories[trajectories.size() - 1];
	}

	// Calculate Retrace: Qret
	auto predictedActions = newActorCritic->Forward(trajectory.newStates);
	torch::Tensor Qret = (predictedActions.first * predictedActions.second).data().sum(-1).unsqueeze(-1);

	// Loop through trajectories
	int i = (onPolicy) ? trajectories[0].numOfTransitions - 1 : trajectories.size() - 1;
	for (; i >= 0; --i)
	{
		if (onPolicy)
		{
			trajectory.states[0] = trajectories[0].states[i];
			trajectory.actions[0] = trajectories[0].actions[i];
			trajectory.rewards[0] = trajectories[0].rewards[i];
			trajectory.newStates[0] = trajectories[0].newStates[i];
			trajectory.terminals[0] = trajectories[0].terminals[i];
			trajectory.policy[0] = trajectories[0].policy[i];
		}
		else
		{
			trajectory = trajectory = trajectories[i];
		}

		// Calculate state-action values 
		predictedActions = newActorCritic->Forward(trajectory.states);
		auto avgPredictedActions = averageActorCritic->Forward(trajectory.states);

		// ρi ← f(ai | φθ′(xi)) / μ(ai | xi)
		torch::Tensor importanceWeights = predictedActions.first.data() / trajectory.policy;

		// V ← the expection of Q under π
		torch::Tensor value = (predictedActions.first * predictedActions.second).data().sum(-1).unsqueeze(-1) * (1.0f - trajectory.terminals);

		// Qret ← r_i + γQret (If terminal is true, Qret equals 0)
		Qret = trajectory.rewards + gamma * Qret * (1.0f - trajectory.terminals);

		// Advantage A ← Qret - V(s_i; θ)
		torch::Tensor QretAdvantage = Qret - value;

		// Actor Loss
		// g ← min(c, ρ_a_i)∙∇θ∙log(π(a_i|s_i; θ))∙A
		torch::Tensor actionIndices = trajectory.actions.toType(torch::kInt64);
		torch::Tensor actorLoss =
			importanceWeights.gather(-1, actionIndices.data()).clamp(0, truncationParameter)
			* -actorLossWeight
			* predictedActions.first.gather(-1, actionIndices).log()
			* QretAdvantage;

		// g = g + Σ_a [1 - c/ρ_a]_+∙π(a|s_i; θ)∙∇θ∙log(π(a|s_i; θ))∙(Q(s_i, a; θ) - V(s_i; θ)
		actorLoss +=
			((1 - truncationParameter / importanceWeights).clamp(0)
				* predictedActions.first.data()
				* -actorLossWeight
				* predictedActions.first.log()
				* (predictedActions.second.gather(-1, actionIndices).data() - value)).sum(-1).unsqueeze(-1);

		// Calculate gradients
		std::vector<torch::Tensor> actorGradients = torch::autograd::grad({ actorLoss.mean() }, { predictedActions.first }, {}, /*retain_graph=*/true);

		// Trust Region update
		std::vector<torch::Tensor> trustRegionGrad = TrustRegion(actorGradients, predictedActions.first, avgPredictedActions.first.data());
		predictedActions.first.backward(trustRegionGrad[0], true);
	
		// Critic
		// dθ ← (Qret - Q(s_i, a_i; θ)) ^ 2
		torch::Tensor criticLoss = (predictedActions.second.gather(-1, actionIndices) - Qret).pow(2);

		// dθ ← dθ - ∇θ
		criticLoss.mean().backward({}, true);

		// Entropy Regularization
		// dθ ← dθ + β∙∇θH(π(s_i; θ))
		torch::Tensor entropyLoss = 1e-3 * (predictedActions.first * predictedActions.first.log()).sum(-1);
		entropyLoss.mean().backward({}, true);

		// Update retrace
		// Qret ← ρ¯_a_i∙(Qret - Q(s_i, a_i; θ)) + V(s_i; θ)
		Qret =
			importanceWeights.gather(-1, actionIndices.data()).clamp({}, 1)
			* (Qret - predictedActions.second.gather(-1, actionIndices).data())
			+ value;
	}

	// Update networks
	actorCritic->CopyGradientsFrom(*newActorCritic);
	actorCritic->GetOptimizer()->step();
	averageActorCritic->CopyParametersFrom(*actorCritic, trustRegionDecay);
}

std::vector<torch::Tensor> ACERAgent::TrustRegion(std::vector<torch::Tensor> gradients,
	torch::Tensor policy, torch::Tensor averagePolicy)
{
	// KL divergence k ← ∇θ0∙DKL[π(∙|s_i; θ_a) || π(∙|s_i; θ)]
	torch::Tensor kullbackLeibler = -((averagePolicy.log() - policy.log()) * averagePolicy).sum(-1);
	std::vector<torch::Tensor> kullbackLeiblerGrad = torch::autograd::grad({ kullbackLeibler.mean() }, { policy }, {}, true);

	// Update Gradients by performing the dot product of each of the
	// gradients and then computing the trust region update
	unsigned int i = 0;
	std::vector<torch::Tensor> updatedGradients;
	for (torch::Tensor actorGrad : gradients)
	{
		torch::Tensor scale = actorGrad.mul(kullbackLeiblerGrad[i]).sum(-1).unsqueeze(-1) - trustRegionConstraint;
		scale = torch::div(scale, actorGrad.mul(actorGrad).sum(-1).unsqueeze(-1)).clamp(0);
		updatedGradients.push_back(actorGrad - scale * kullbackLeiblerGrad[i]);
		i++;
	}

	return updatedGradients;
}
