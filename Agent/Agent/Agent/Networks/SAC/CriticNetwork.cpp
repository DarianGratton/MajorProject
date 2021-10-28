#include "CriticNetwork.h"

CriticNetwork::CriticNetwork(
	float lr, unsigned int nActions,
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims) :
	learningRate(lr), numActions(nActions),
	inputDims(inputDims), layer1Dims(layer1Dims), layer2Dims(layer2Dims)
{
	// Create layers
	layer1 = torch::nn::Linear(inputDims + numActions, layer1Dims);
	layer2 = torch::nn::Linear(layer1Dims, layer2Dims);
	q = torch::nn::Linear(layer2Dims, 1); // 1 Scalar output

	optimizer = new torch::optim::Adam(parameters(), learningRate);
}

torch::Tensor CriticNetwork::Forward(at::Tensor state, at::Tensor action)
{
	torch::Tensor actionValue = layer1(torch::cat({state, action}, 1));
	actionValue = torch::nn::functional::relu(actionValue);
	actionValue = layer2(actionValue);
	actionValue = torch::nn::functional::relu(actionValue);
	
	return q(actionValue);
}

// On hold until ReplayMemory is completed
void CriticNetwork::SaveMemory()
{

}

void CriticNetwork::LoadMemory()
{

}
