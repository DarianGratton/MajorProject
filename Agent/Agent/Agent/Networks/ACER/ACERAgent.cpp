#include "ACERAgent.h"

ACERAgent::ACERAgent(float lr,
	unsigned int nActions, unsigned int nPossibleActions,
	int64_t inputDims, unsigned int nHiddenLayers, 
	int64_t hiddenLayerDims, int64_t actionLayerDims,
	unsigned int memSize /* = 10000 */, unsigned int maxEpisodeLength /* = 256 */, 
	unsigned int batchSize /* = 16 */, unsigned int batchTrajectoryLength /* = 16 */,
	float biasWeight /* = 0.1f */, float gamma /* = 0.99f */, 
	int traceMax /* = 10 */, float trustRegionConstraint /* = 1.0f */, 
	float trustRegionDecay /* = 0.99f */) :
	batchSize(batchSize), biasWeight(biasWeight), 
	gamma(gamma), traceMax(traceMax),
	trustRegionConstraint(trustRegionConstraint), trustRegionDecay(trustRegionDecay)
{
	// Initialize Networks
	actorCritic = ActorCriticNetwork(lr, nPossibleActions, inputDims, nHiddenLayers, hiddenLayerDims, actionLayerDims);
	averageActorCritic = actorCritic->CleanClone();

	// Initialize Memory
	memory = unique_ptr<ACERReplayMemory>(new ACERReplayMemory(memSize, maxEpisodeLength, inputDims, nActions, nPossibleActions));
}

vector<float> ACERAgent::PredictAction(torch::Tensor state)
{
	// Predict Probabilites
	auto predictedAction = actorCritic->Forward(state);

	// Get Action
	// TODO: Modify for multiple actions (Out-of-scope)
	float action = predictedAction.first.multinomial(1).item<float>();

	return { action };
}

void ACERAgent::UpdateMemory(
	State state,
	vector<float> actions,
	float reward,
	State newState,
	bool terminal)
{
	// Get Current Policy
	auto pred = actorCritic->Forward(state.ToTensor());
	vector<float> policy;
	for (int i = 0; i < pred.first.size(1); i++)
	{
		policy.push_back(pred.first[0][i].item<float>());
	}

	// Insert Transition into memory
	memory->StoreStateTransition(state, actions, reward, newState, terminal, policy);
}

void ACERAgent::Learn()
{
	// On-Policy
	Learn({ memory->GetPrevTrajectory() }, true);

	// Off-Policy
	if (memory->GetCurrentMemsize() >= batchSize)
	{
		vector<Trajectory> trajectories = memory->SampleMemory(batchSize, batchTrajectoryLength);
		Learn(trajectories, false);
	}
}

void ACERAgent::SaveModel()
{
	// Save checkpoint
	actorCritic->GetCheckpoint().str(string());
	torch::save(actorCritic, actorCritic->GetCheckpoint());

	averageActorCritic->GetCheckpoint().str(string());
	torch::save(averageActorCritic, averageActorCritic->GetCheckpoint());

	// Save checkpoint to file
	ofstream outFile;
	outFile.open("ACERActorCritic.txt", ios::binary | ios::trunc);
	outFile << actorCritic->GetCheckpoint().rdbuf();
	outFile.close();

	outFile.open("ACERAvgActorCritic.txt", ios::binary | ios::trunc);
	outFile << averageActorCritic->GetCheckpoint().rdbuf();
	outFile.close();
}

void ACERAgent::LoadModel()
{
	// Clear the Checkpoint
	actorCritic->GetCheckpoint().str(string());
	averageActorCritic->GetCheckpoint().str(string());

	// Load Checkpoint from file
	ifstream inFile;
	inFile.open("ACERActorCritic.txt", ios::binary);
	actorCritic->GetCheckpoint() << inFile.rdbuf();
	inFile.close();

	inFile.open("ACERAvgActorCritic.txt", ios::binary);
	averageActorCritic->GetCheckpoint() << inFile.rdbuf();
	inFile.close();

	// Load Checkpoint
	torch::load(actorCritic, actorCritic->GetCheckpoint());
	torch::load(averageActorCritic, averageActorCritic->GetCheckpoint());
}

void ACERAgent::Learn(vector<Trajectory> trajectories, bool onPolicy)
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
			importanceWeights.gather(-1, actionIndices.data()).clamp(0, traceMax)
			* -biasWeight
			* predictedActions.first.gather(-1, actionIndices).log()
			* QretAdvantage;

		// g = g + Σ_a [1 - c/ρ_a]_+∙π(a|s_i; θ)∙∇θ∙log(π(a|s_i; θ))∙(Q(s_i, a; θ) - V(s_i; θ)
		actorLoss +=
			((1 - traceMax / importanceWeights).clamp(0)
				* predictedActions.first.data()
				* -biasWeight
				* predictedActions.first.log()
				* (predictedActions.second.gather(-1, actionIndices).data() - value)).sum(-1).unsqueeze(-1);

		// Calculate gradients
		vector<torch::Tensor> actorGradients = torch::autograd::grad({ actorLoss.mean() }, { predictedActions.first }, {}, /*retain_graph=*/true);

		// Trust Region update
		vector<torch::Tensor> trustRegionGrad = TrustRegion(actorGradients, predictedActions.first, avgPredictedActions.first.data());
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

vector<torch::Tensor> ACERAgent::TrustRegion(vector<torch::Tensor> gradients,
	torch::Tensor policy, torch::Tensor averagePolicy)
{
	// KL divergence k ← ∇θ0∙DKL[π(∙|s_i; θ_a) || π(∙|s_i; θ)]
	torch::Tensor kullbackLeibler = -((averagePolicy.log() - policy.log()) * averagePolicy).sum(-1);
	vector<torch::Tensor> kullbackLeiblerGrad = torch::autograd::grad({ kullbackLeibler.mean() }, { policy }, {}, true);

	// Update Gradients by performing the dot product of each of the
	// gradients and then computing the trust region update
	unsigned int i = 0;
	vector<torch::Tensor> updatedGradients;
	for (torch::Tensor actorGrad : gradients)
	{
		torch::Tensor scale = actorGrad.mul(kullbackLeiblerGrad[i]).sum(-1).unsqueeze(-1) - trustRegionConstraint;
		scale = torch::div(scale, actorGrad.mul(actorGrad).sum(-1).unsqueeze(-1)).clamp(0);
		updatedGradients.push_back(actorGrad - scale * kullbackLeiblerGrad[i]);
		i++;
	}

	return updatedGradients;
}
