#pragma once

#include <torch/torch.h>

// TODO: Implement functionality for a basic Policy Network
class PolicyNetwork : torch::nn::Module 
{
public:

	PolicyNetwork(float lr, 
		unsigned int nActions, unsigned int maxActions,
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	std::pair<torch::Tensor, torch::Tensor> Forward(torch::Tensor state);
	void CalculateActionProb(torch::Tensor state, bool reparam = true);
	
	void SaveMemory();
	void LoadMemory();

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
};
