#pragma once

#include <torch/torch.h>

#include <sstream>

// TODO: Implement functionality for a basic Value Network
class ValueNetworkImpl : public torch::nn::Module
{
public:
	ValueNetworkImpl(float lr, int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	torch::Tensor Forward(torch::Tensor state);

	inline std::stringstream& GetCheckpoint() { return checkpoint; }

private:
	float learningRate;

	// Network layers
	torch::nn::Linear layer1 = nullptr;
	torch::nn::Linear layer2 = nullptr;
	torch::nn::Linear value = nullptr;
	torch::optim::Adam* optimizer;

	// Checkpoints
	std::stringstream checkpoint;
};
TORCH_MODULE(ValueNetwork);
