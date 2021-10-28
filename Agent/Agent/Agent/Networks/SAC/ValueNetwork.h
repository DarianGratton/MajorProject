#pragma once

#include <torch/torch.h>

// TODO: Implement functionality for a basic Value Network
class ValueNetwork : torch::nn::Module 
{
public:

	ValueNetwork(float lr, int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	torch::Tensor Forward(torch::Tensor state);

	void SaveMemory();
	void LoadMemory();

private:
	float learningRate;

	// Network layers
	torch::nn::Linear layer1 = nullptr;
	torch::nn::Linear layer2 = nullptr;
	torch::nn::Linear value = nullptr;
	torch::optim::Adam* optimizer;
};
