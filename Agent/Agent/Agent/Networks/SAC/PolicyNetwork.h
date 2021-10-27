#pragma once

#include <torch/torch.h>

// TODO: Implement functionality for a basic Policy Network
class PolicyNetwork : torch::nn::Module 
{
public:

	PolicyNetwork();

	torch::Tensor Forward();
	void CalculateActionProb();
	
	void SaveMemory();
	void LoadMemory();

private:
	float learningRate;
	unsigned int maxNumActions;
	unsigned int numActions;
};
