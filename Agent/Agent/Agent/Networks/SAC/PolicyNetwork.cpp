#include "PolicyNetwork.h"

#include "../../PyTorchExpansion/TorchNormal.h"

PolicyNetworkImpl::PolicyNetworkImpl(float lr,
	unsigned int nActions, unsigned int maxActions,
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims) :
	learningRate(lr), numActions(nActions), maxNumActions(maxActions),
	layer1(torch::nn::Linear(inputDims, layer1Dims)),
	layer2(torch::nn::Linear(layer1Dims, layer2Dims)),
	mu(torch::nn::Linear(layer2Dims, nActions)),
	sigma(torch::nn::Linear(layer2Dims, nActions))
{
	// Register modules (Needed for parameters())
	register_module("layer1", layer1);
	register_module("layer2", layer2);
	register_module("mu", mu);
	register_module("sigma", sigma);
	
	// Create optimizer
	optimizer = new torch::optim::Adam(parameters(), learningRate);
}

std::pair<torch::Tensor, torch::Tensor> PolicyNetworkImpl::Forward(torch::Tensor state)
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

std::pair<torch::Tensor, torch::Tensor> PolicyNetworkImpl::CalculateActionProb(torch::Tensor state, bool reparam)
{
	// Get network output
	std::pair<torch::Tensor, torch::Tensor> networkOutput;
	networkOutput = Forward(state);
	torch::Tensor mu = networkOutput.first;
	torch::Tensor sigma = networkOutput.second;

	// Create object and get sample
	TorchNormal probabilities(mu, sigma);
	torch::Tensor actions = (reparam) ? probabilities.RSample() : probabilities.Sample();
	
	// TODO: Possibly for discrete action spaces only
	// Create data container
	std::vector<int> possibleActions(maxNumActions);
	std::iota(std::begin(possibleActions), std::end(possibleActions), 0);
	torch::detail::TensorDataContainer data(possibleActions);

	// Calculate action probability
	torch::Tensor action = torch::tanh(actions) * torch::tensor(data);
	torch::Tensor logProbs = probabilities.LogProb(actions);
	logProbs -= torch::log(1 - action.pow(2) + reparamNoise); // How the paper handles the scaling of the action
	logProbs = logProbs.sum(1, true);

	return std::make_pair(action, logProbs);
}
