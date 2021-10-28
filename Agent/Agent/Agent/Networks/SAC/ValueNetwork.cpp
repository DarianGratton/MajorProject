#include "ValueNetwork.h"

ValueNetwork::ValueNetwork(float lr, 
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims) :
	learningRate(lr)
{
	// Create layers
	layer1 = torch::nn::Linear(inputDims, layer1Dims);
	layer2 = torch::nn::Linear(layer1Dims, layer2Dims);
	value = torch::nn::Linear(layer2Dims, 1); // 1 Scalar output

	optimizer = new torch::optim::Adam(parameters(), learningRate);
}

torch::Tensor ValueNetwork::Forward(torch::Tensor state)
{
	torch::Tensor stateValue = layer1(state);
	stateValue = torch::nn::functional::relu(stateValue);
	stateValue = layer2(stateValue);
	stateValue = torch::nn::functional::relu(stateValue);

	return value(stateValue);
}

void ValueNetwork::SaveMemory()
{

}

void ValueNetwork::LoadMemory()
{

}
