#pragma once

#include <torch/torch.h>

#include <vector>

using namespace std;

class TorchDistribution
{
protected:
	vector<int64_t> batch_shape, event_shape;

	vector<int64_t> extended_shape(c10::ArrayRef<int64_t> sample_shape);

public:
	virtual torch::Tensor Sample() = 0;
	virtual torch::Tensor RSample() = 0;
	virtual torch::Tensor LogProb(torch::Tensor value) = 0;
};
