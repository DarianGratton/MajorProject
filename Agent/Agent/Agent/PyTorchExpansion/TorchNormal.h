#pragma once

#include <vector>

#include <torch/torch.h>

using namespace std;

class TorchNormal
{
public:
	TorchNormal(torch::Tensor mean, torch::Tensor stddev);

	torch::Tensor Sample();
	torch::Tensor RSample();
	torch::Tensor LogProb(torch::Tensor value);

private:
	// Mean of the distribution (mu)
	torch::Tensor mean;

	// Standard deviation (sigma)
	torch::Tensor stddev;

	// Variance
	torch::Tensor variance;
};
