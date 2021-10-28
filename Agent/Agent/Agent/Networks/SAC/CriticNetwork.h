#pragma once

#include <torch/torch.h>

// TODO: Implement functionality for a basic Critic Network
class CriticNetwork : torch::nn::Module 
{
public:

	CriticNetwork(float lr, unsigned int nActions, 
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	torch::Tensor Forward(torch::Tensor state, torch::Tensor action);

	void SaveMemory();
	void LoadMemory();

private:
	float learningRate;
	unsigned int numActions;

	// Network layers
	torch::nn::Linear layer1 = nullptr;
	torch::nn::Linear layer2 = nullptr;
	torch::nn::Linear q = nullptr;
	torch::optim::Adam* optimizer;
};
