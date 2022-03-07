#include "PolicyNetwork.h"

#include "../../PyTorchExpansion/TorchNormal.h"

PolicyNetworkImpl::PolicyNetworkImpl(float lr, 
	unsigned int nActions, unsigned int maxNActions,
	int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims) :
	learningRate(lr), numActions(nActions), 
	maxNumActions(maxNActions),
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
	to(torch::kCPU);
}

std::pair<torch::Tensor, torch::Tensor> PolicyNetworkImpl::Forward(torch::Tensor state)
{
	try 
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
	catch (const c10::Error& e)
	{
		std::cout << e.msg() << std::endl;
	}

	return std::make_pair(torch::Tensor(), torch::Tensor());
}

std::pair<torch::Tensor, torch::Tensor> PolicyNetworkImpl::CalculateActionProb(torch::Tensor state, bool reparam)
{
	try
	{
		// Get network output
		std::pair<torch::Tensor, torch::Tensor> networkOutput;
		networkOutput = Forward(state);
		torch::Tensor muOutput = networkOutput.first;
		torch::Tensor sigmaOutput = networkOutput.second;

		// Create object and get sample
		TorchNormal probabilities(muOutput, sigmaOutput);
		torch::Tensor actions = (reparam) ? probabilities.RSample() : probabilities.Sample();

		// TODO: Possibly for discrete action spaces only
		// Create data container
		std::vector<int> possibleActions(maxNumActions, 1);
		torch::detail::TensorDataContainer data(possibleActions);

		// Note: Should work based on testing, unknown if there is currently a problem. Possible spot to return to if agent fails.
		// Calculate action probability
		torch::Tensor action = torch::tanh(actions);

		torch::Tensor logProbs = probabilities.LogProb(actions);
		logProbs -= torch::log(1 - action.pow(2) + reparamNoise); // How the paper handles the scaling of the action
		logProbs *= torch::tensor(data, torch::kF32);
		logProbs = logProbs.sum(1, true);
	
		return std::make_pair(action, logProbs);
	}
	catch (const c10::Error& e)
	{
		std::cerr << "CalculateActionProb error: " << e.msg() << std::endl;
		return std::make_pair(torch::Tensor(), torch::Tensor());
	}
}
