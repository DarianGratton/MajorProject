#include "ActorCriticNetwork.h"

ActorCriticNetworkImpl::ActorCriticNetworkImpl(
	float lr, unsigned int nPossibleActions, 
	int64_t inputDims, unsigned int nHiddenLayers, 
	int64_t hiddenLayerDims, int64_t actionLayerDims) :
	learningRate(lr), nPossibleActions(nPossibleActions),
	inputDims(inputDims), nHiddenLayers(nHiddenLayers),
	hiddenLayerDims(hiddenLayerDims), actionLayerDims(actionLayerDims)
{
	// Create network layers and register modules
	inputLayer = register_module("inputLayer", torch::nn::Linear(inputDims, hiddenLayerDims));

	for (int i = 0; i < nHiddenLayers; i++)
	{
		torch::nn::Linear hiddenLayer = nullptr;
		std::string layerName = "hiddenLayer" + std::to_string(i + 1);

		if (i == (nHiddenLayers - 1))
			hiddenLayer = register_module(layerName.c_str(), torch::nn::Linear(hiddenLayerDims, actionLayerDims));
		else
			hiddenLayer = register_module(layerName.c_str(), torch::nn::Linear(hiddenLayerDims, hiddenLayerDims));
		
		hiddenLayers.push_back(hiddenLayer);
	}

	actionLayer = register_module("actionLayer", torch::nn::Linear(actionLayerDims, nPossibleActions));
	actionValueLayer = register_module("actionValueLayer", torch::nn::Linear(actionLayerDims, nPossibleActions));

	// Create Optimizer
	optimizer = new torch::optim::Adam(parameters(), learningRate);
	
	// Register network to device
	to(torch::kCPU);
}

std::pair<torch::Tensor, torch::Tensor> ActorCriticNetworkImpl::Forward(torch::Tensor state)
{
	torch::Tensor hidden = inputLayer(state);
	hidden = torch::nn::functional::relu(hidden);

	for (auto hiddenLayer : hiddenLayers)
	{
		hidden = hiddenLayer(hidden);
		hidden = torch::nn::functional::relu(hidden);
	}

	torch::Tensor actionProbs = torch::nn::functional::softmax(actionLayer(hidden), -1);
	torch::Tensor actionValues = actionValueLayer(hidden);

	return std::make_pair(actionProbs, actionValues);
}

void ActorCriticNetworkImpl::CopyParametersFrom(const ActorCriticNetworkImpl& source, float decay /* = 0 */)
{
	unsigned int i = 0;
	std::vector<torch::Tensor> sourceParameters = source.parameters();
	for (torch::Tensor localParameter : this->parameters())
	{
		localParameter.data().copy_(decay * localParameter.data() + (1 - decay) * sourceParameters[i].data());
		i++;
	}
}

void ActorCriticNetworkImpl::CopyGradientsFrom(const ActorCriticNetworkImpl& source)
{
	unsigned int i = 0;
	std::vector<torch::Tensor> sourceParameters = source.parameters();
	for (torch::Tensor localParameter : this->parameters())
	{
		localParameter.mutable_grad() = sourceParameters[i].grad();
		i++;
	}
}

std::shared_ptr<ActorCriticNetworkImpl> ActorCriticNetworkImpl::CleanClone()
{
	return std::make_shared<ActorCriticNetworkImpl>(
		learningRate, nPossibleActions,
		inputDims, nHiddenLayers,
		hiddenLayerDims, actionLayerDims);
}
