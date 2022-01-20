#include "ValueNetwork.h"

ValueNetworkImpl::ValueNetworkImpl(float lr,
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims) :
	learningRate(lr),
	layer1(torch::nn::Linear(inputDims, layer1Dims)),
	layer2(torch::nn::Linear(layer1Dims, layer2Dims)),
	value(torch::nn::Linear(layer2Dims, 1))
{
	// Register modules (Needed for parameters())
	register_module("layer1", layer1);
	register_module("layer2", layer2);
	register_module("value", value);

	// Create optimizer
	optimizer = new torch::optim::Adam(parameters(), learningRate);
}

torch::Tensor ValueNetworkImpl::Forward(torch::Tensor state)
{
	torch::Tensor stateValue = layer1(state);
	stateValue = torch::nn::functional::relu(stateValue);
	stateValue = layer2(stateValue);
	stateValue = torch::nn::functional::relu(stateValue);

	return value(stateValue);
}
