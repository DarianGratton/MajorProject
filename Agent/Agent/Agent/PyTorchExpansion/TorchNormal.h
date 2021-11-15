#pragma once

#include <vector>

#include <torch/torch.h>

using namespace std;

class TorchNormal
{
public:
	TorchNormal(torch::Tensor loc, torch::Tensor scale);

	torch::Tensor Sample(int64_t sampleShape);

private:
	// Mean of the distribution
	torch::autograd::variable_list loc;

	// Standard deviation
	torch::autograd::variable_list scale;

	// Todo: Comment what this is
	size_t batchSize;
	size_t eventSize;
};
