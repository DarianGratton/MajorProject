#include "TorchNormal.h"

TorchNormal::TorchNormal(torch::Tensor mean, torch::Tensor stddev) :
	mean(mean), stddev(stddev)
{
	variance = stddev.pow(2); 
}

torch::Tensor TorchNormal::Sample()
{
	torch::NoGradGuard noGrad;
	return torch::randn({ 1 }) * stddev + mean;
}

torch::Tensor TorchNormal::RSample()
{
	torch::Tensor eps = torch::empty(mean.sizes(), mean.options());
	return mean + eps * stddev;
}

torch::Tensor TorchNormal::LogProb(torch::Tensor value)
{
	torch::Tensor logScale = stddev.log();
	return -(torch::pow((value - mean), 2) / (2 * variance) - logScale - std::log(std::sqrt(2 * M_PI)));
}
