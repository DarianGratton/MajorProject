#pragma once

#include <torch/torch.h>

#include <sstream>

/// <summary>
/// 
/// Policy Network for the Soft-Actor Critic Algorithm. 
/// Takes a state and determines what actions the 
/// actor takes in an give state.
/// 
/// Author: Darian G.
/// 
/// </summary>
class PolicyNetworkImpl : public torch::nn::Module 
{
public:
	/// <summary>
	/// Constructor for the Policy Network class.
	/// </summary>
	/// <param name="lr">Learning rate of the network.</param>
	/// <param name="nActions">Number of actions the agent can take.</param>
	/// <param name="maxNumActions">Max number of actions the agent can possibly take.</param>
	/// <param name="inputDims">Input dimensions (size of the state).</param>
	/// <param name="layer1Dims">Layer 1 dimenstions.</param>
	/// <param name="layer2Dims">Layer 2 dimenstions.</param>
	PolicyNetworkImpl(float lr, 
		unsigned int nActions, unsigned int maxNumActions,
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	/// <summary>
	/// Forward of the network, takes a state and returns a output.
	/// </summary>
	/// <param name="state">The current state.</param>
	/// <returns>Tensor value of what action the tensor should take.</returns>
	std::pair<torch::Tensor, torch::Tensor> Forward(torch::Tensor state);

	/// <summary>
	/// Calculates the policy.
	/// 
	/// TODO: Comment is vague and function should be renamed.
	/// </summary>
	/// <param name="state"></param>
	/// <param name="reparam"></param>
	/// <returns></returns>
	std::pair<torch::Tensor, torch::Tensor> CalculateActionProb(torch::Tensor state, bool reparam = true);
	
	inline std::stringstream& GetCheckpoint() { return checkpoint; }
	inline torch::optim::Adam* GetOptimizer() { return optimizer; }

private:
	const float reparamNoise = 1e-6;
	float learningRate;
	unsigned int numActions;
	unsigned int maxNumActions;

	// Network layers
	torch::nn::Linear layer1 = nullptr;
	torch::nn::Linear layer2 = nullptr;
	torch::nn::Linear mu = nullptr;
	torch::nn::Linear sigma = nullptr;
	torch::optim::Adam* optimizer;

	// Checkpoints
	std::stringstream checkpoint;
};
TORCH_MODULE(PolicyNetwork);
