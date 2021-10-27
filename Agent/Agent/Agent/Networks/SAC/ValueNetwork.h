#pragma once

#include <torch/torch.h>

// TODO: Implement functionality for a basic Value Network
class ValueNetwork : torch::nn::Module 
{
public:

	ValueNetwork();

	torch::Tensor Forward();

	void SaveMemory();
	void LoadMemory();

private:
	float learningRate;
};
