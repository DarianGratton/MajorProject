#pragma once

#include <torch/torch.h>

#include <sstream>

// TODO: Implement functionality for a basic Policy Network
class PolicyNetworkImpl : public torch::nn::Module 
{
public:
	PolicyNetworkImpl(float lr,
		unsigned int nActions, unsigned int maxActions,
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	std::pair<torch::Tensor, torch::Tensor> Forward(torch::Tensor state);
	std::pair<torch::Tensor, torch::Tensor> CalculateActionProb(torch::Tensor state, bool reparam = true);
	
	inline std::stringstream& GetCheckpoint() { return checkpoint; }

private:
	const float reparamNoise = 1e-6;
	float learningRate;
	unsigned int maxNumActions;
	unsigned int numActions;

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
