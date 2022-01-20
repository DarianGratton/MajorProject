#pragma once

#include <torch/torch.h>

#include <sstream>

// TODO: Implement functionality for a basic Critic Network
class CriticNetworkImpl : public torch::nn::Module 
{
public:

	CriticNetworkImpl(float lr, unsigned int nActions,
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	torch::Tensor Forward(torch::Tensor state, torch::Tensor action);

	inline std::stringstream& GetCheckpoint() { return checkpoint; }

private:
	float learningRate;
	unsigned int numActions;

	// Network layers
	torch::nn::Linear layer1 = nullptr;
	torch::nn::Linear layer2 = nullptr;
	torch::nn::Linear q = nullptr;
	torch::optim::Adam* optimizer;

	// Checkpoints
	std::stringstream checkpoint;
};
TORCH_MODULE(CriticNetwork);
