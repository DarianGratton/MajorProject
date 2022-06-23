#include "ACERAgent.h"

#include <typeinfo>

ACERAgent::ACERAgent(float lr,
	unsigned int nActions, unsigned int nPossibleActions,
	int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims,
	unsigned int memSize /* = 10000 */, unsigned int maxEpisodeLength /* = 256 */, 
	unsigned int batchSize /* = 16 */, float biasWeight /* = 0.1f */, 
	float gamma /* = 0.99f */, int traceMax /* = 10 */) :
	batchSize(batchSize), biasWeight(biasWeight), 
	gamma(gamma), traceMax(traceMax)
{
	// TODO: Need to call share_memory() or something, but C++ API doesn't have it
	// should figure out how to implement when doing multithreading
	actorCritic = ActorCriticNetwork(lr, nPossibleActions, inputDims, hiddenLayerDims, actionLayerDims);
	averageActorCritic = actorCritic->CleanClone();
	memory = unique_ptr<ACERReplayMemory>(new ACERReplayMemory(memSize, maxEpisodeLength, inputDims, nActions, nPossibleActions));

	// torch::autograd::AnomalyMode::set_enabled(true);
}

ACERAgent::~ACERAgent()
{
	
}

void ACERAgent::Run(TestEnvironment& env, 
	unsigned int nEpisodes, unsigned int episodeLength)
{
	for (int episode = 1; episode <= nEpisodes; episode++)
	{

	}
}

std::pair<int, std::vector<float>> ACERAgent::PredictAction(State observation)
{
	// Predict Probabilites
	/*torch::Tensor state = torch::nn::functional::normalize(observation.ToTensor(),
							torch::nn::functional::NormalizeFuncOptions().p(1).dim(1));*/
	auto predictedAction = actorCritic->Forward(observation.ToTensor());
	
	// Get Action
	int action = predictedAction.first.multinomial(1).item<int>();
	
	// Store Probabilites in Vector
	std::vector<float> actionProbabilites;
	for (int i = 0; i < predictedAction.first.size(1); i++)
	{
		actionProbabilites.push_back(predictedAction.first[0][i].item<float>());
	}
	
	return std::make_pair(action, actionProbabilites);
}

void ACERAgent::UpdateMemory(
	State state,
	vector<int> actions,
	float reward,
	State newState,
	bool terminal,
	vector<float> actionProbabilities)
{
	memory->StoreStateTransition(state, actions, reward, newState, terminal, actionProbabilities);
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

	outFile.open("ACERAvgActorCritic.pt", ios::binary | ios::trunc);
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

	inFile.open("ACERAvgActorCritic.pt", ios::binary);
	averageActorCritic->GetCheckpoint() << inFile.rdbuf();
	inFile.close();

	// Load Checkpoint
	torch::load(actorCritic, actorCritic->GetCheckpoint());
	torch::load(averageActorCritic, averageActorCritic->GetCheckpoint());
}

void ACERAgent::Learn(std::vector<Trajectory> trajectories)
{
	// Reset gradients and initialize models parameters
	auto newActorCritic = actorCritic->CleanClone();
	newActorCritic->CopyParametersFrom(*actorCritic);
	actorCritic->GetOptimizer()->zero_grad();

	// Calculate Retrace: Qret
	Trajectory trajectory = trajectories[trajectories.size() - 1];
	auto predictedActions = newActorCritic->Forward(trajectory.newStates);
	torch::Tensor Qret = (predictedActions.first * predictedActions.second).data().sum(-1).unsqueeze(-1);

	// Loop through trajectories
	for (int i = trajectories.size() - 1; i >= 0; --i)
	{
		// Calculate state-action values 
		trajectory = trajectories[i];
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
		// actorLoss = -(actorLoss);

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

		// Entropy
		// TODO: 1e-3 should be a variable called entropy regularization or something
		// TODO: Figure out formula this even follows
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
	// TODO: Replace 0.99f with a variable.
	actorCritic->CopyGradientsFrom(*newActorCritic);

	// Optimizer
	actorCritic->GetOptimizer()->step();

	averageActorCritic->CopyParametersFrom(*actorCritic, 0.99f);
}

void ACERAgent::OnPolicyLearn(std::vector<Trajectory> trajectories)
{
	// Reset gradients and initialize models parameters
	auto newActorCritic = actorCritic->CleanClone();
	newActorCritic->CopyParametersFrom(*actorCritic);
	actorCritic->GetOptimizer()->zero_grad();

	for (Trajectory transition : trajectories)
	{
		// Calculate state-action values 
		auto predictedActions = newActorCritic->Forward(transition.states);
		auto avgPredictedActions = averageActorCritic->Forward(transition.states);

		// 
	}
}

// TODO: - 1 on the first line of the for loop should be a variable TRUST_REGION_CONSTRAINT
std::vector<torch::Tensor> ACERAgent::TrustRegion(std::vector<torch::Tensor> gradients,
	torch::Tensor policy, torch::Tensor averagePolicy)
{
	torch::Tensor kullbackLeibler = -((averagePolicy.log() - policy.log()) * averagePolicy).sum(-1);
	std::vector<torch::Tensor> kullbackLeiblerGrad = torch::autograd::grad({ kullbackLeibler.mean() }, { policy }, {}, true);

	unsigned int i = 0;
	std::vector<torch::Tensor> updatedGradients;
	for (torch::Tensor actorGrad : gradients)
	{
		torch::Tensor scale = actorGrad.mul(kullbackLeiblerGrad[i]).sum(-1).unsqueeze(-1) - 1;
		scale = torch::div(scale, actorGrad.mul(actorGrad).sum(-1).unsqueeze(-1)).clamp(0);
		updatedGradients.push_back(actorGrad - scale * kullbackLeiblerGrad[i]);
		i++;
	}

	return updatedGradients;
}
