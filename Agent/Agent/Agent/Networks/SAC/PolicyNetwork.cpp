#include "PolicyNetwork.h"

PolicyNetwork::PolicyNetwork(float lr, 
	unsigned int nActions, unsigned int maxActions,
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims) :
	learningRate(lr), numActions(nActions), maxNumActions(maxActions)
{
	// Set up network
	layer1 = torch::nn::Linear(inputDims, layer1Dims);
	layer2 = torch::nn::Linear(layer1Dims, layer2Dims);
	mu = torch::nn::Linear(layer2Dims, numActions);
	sigma = torch::nn::Linear(layer2Dims, numActions);
	
	optimizer = new torch::optim::Adam(parameters(), learningRate);
}

std::pair<torch::Tensor, torch::Tensor> PolicyNetwork::Forward(torch::Tensor state)
{
	torch::Tensor prob = layer1(state);
	prob = torch::nn::functional::relu(prob);
	prob = layer2(prob);
	prob = torch::nn::functional::relu(prob);

	torch::Tensor muOutput = mu(prob);
	torch::Tensor sigmaOutput = sigma(prob);

	sigmaOutput = torch::clamp(sigmaOutput, c10::Scalar(reparamNoise), c10::Scalar(1));

	return std::make_pair(muOutput, sigmaOutput);
}

void PolicyNetwork::CalculateActionProb()
{

}

void PolicyNetwork::SaveMemory()
{

}

void PolicyNetwork::LoadMemory()
{

}
