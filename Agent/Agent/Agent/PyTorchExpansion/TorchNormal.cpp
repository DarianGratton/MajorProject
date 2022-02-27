#include "TorchNormal.h"

#include <c10/util/ArrayRef.h>

TorchNormal::TorchNormal(torch::Tensor mean, torch::Tensor stddev) :
	mean(mean), stddev(stddev)
{
	variance = stddev.pow(2); 
	batch_shape = mean.sizes().vec();
	event_shape = {};
}

torch::Tensor TorchNormal::Sample()
{
	vector<int64_t> shape = extended_shape({});
	torch::NoGradGuard noGrad;
	return at::normal(mean.expand(shape), stddev.expand(shape));
}

torch::Tensor TorchNormal::RSample()
{
	torch::Tensor eps = torch::empty(mean.sizes(), mean.options()).normal_();
	return mean + eps * stddev;
}

torch::Tensor TorchNormal::LogProb(torch::Tensor value)
{
	torch::Tensor logScale = stddev.log();
	return -torch::pow((value - mean), 2) / (2 * variance) - logScale - std::log(std::sqrt(2 * M_PI));
}
