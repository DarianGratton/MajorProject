#include "CriticNetwork.h"

CriticNetworkImpl::CriticNetworkImpl(
	float lr, unsigned int nActions,
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims) :
	learningRate(lr), numActions(nActions),
	layer1(torch::nn::Linear(inputDims + nActions, layer1Dims)),
	layer2(torch::nn::Linear(layer1Dims, layer2Dims)),
	q(torch::nn::Linear(layer2Dims, 1))
{
	// Register modules (Needed for parameters())
	register_module("layer1", layer1);
	register_module("layer2", layer2);
	register_module("q", q);

	// Create optimizer
	optimizer = new torch::optim::Adam(parameters(), learningRate);
	to(torch::kCPU);
}

torch::Tensor CriticNetworkImpl::Forward(torch::Tensor state, torch::Tensor action)
{
	torch::Tensor actionValue = layer1(torch::cat({state, action}, 1));
	actionValue = torch::nn::functional::relu(actionValue);
	actionValue = layer2(actionValue);
	actionValue = torch::nn::functional::relu(actionValue);
	
	return q(actionValue);
}
