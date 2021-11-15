#include "TorchNormal.h"

TorchNormal::TorchNormal(torch::Tensor loc, torch::Tensor scale)
{
	// Expands tensors without making copies of the data (Torch Broadcasting)
	this->loc = torch::broadcast_tensors(loc);
	this->scale = torch::broadcast_tensors(scale);

	this->batchSize = this->loc.size();
	this->eventSize = 0;
}

// Requires more testing to ensure implementation is right (Review)
torch::Tensor TorchNormal::Sample(int64_t sampleShape)
{
	int64_t shape = sampleShape + this->batchSize + this->eventSize;
	
	torch::NoGradGuard noGrad;
	this->loc.resize(shape);
	this->scale.resize(shape);
	return torch::normal(this->loc.size(), this->scale.size(), 0);
}
