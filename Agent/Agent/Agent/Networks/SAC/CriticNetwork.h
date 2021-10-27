#pragma once

#include <torch/torch.h>

// TODO: Implement functionality for a basic Critic Network
class CriticNetwork : torch::nn::Module 
{
public:

	CriticNetwork();

	torch::Tensor Forward();

	void SaveMemory();
	void LoadMemory();

private:
	float learningRate;
	unsigned int numActions;
};
