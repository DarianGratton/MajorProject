#include "ActorCriticNetwork.h"

ActorCriticNetworkImpl::ActorCriticNetworkImpl(
	float lr, unsigned int nPossibleActions, 
	int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims) : 
	learningRate(lr), nPossibleActions(nPossibleActions),
	inputDims(inputDims), hiddenLayerDims(hiddenLayerDims), actionLayerDims(actionLayerDims),
	inputLayer(torch::nn::Linear(inputDims, hiddenLayerDims)),
	hiddenLayer1(torch::nn::Linear(hiddenLayerDims, actionLayerDims)),
	actionLayer(torch::nn::Linear(actionLayerDims, nPossibleActions)),
	actionValueLayer(torch::nn::Linear(actionLayerDims, nPossibleActions))
{
	// Register modules (Needed for parameters())
	register_module("inputLayer", inputLayer);
	register_module("hiddenLayer1", hiddenLayer1);
	register_module("actionLayer", actionLayer);
	register_module("actionValueLayer", actionValueLayer);
}

std::pair<torch::Tensor, torch::Tensor> ActorCriticNetworkImpl::Forward(torch::Tensor state)
{
	torch::Tensor hidden = inputLayer(state);
	hidden = torch::nn::functional::relu(hidden);
	hidden = hiddenLayer1(hidden);
	hidden = torch::nn::functional::relu(hidden);

	torch::Tensor actionProbs = torch::nn::functional::softmax(actionLayer(hidden), -1);
	torch::Tensor actionValues = actionValueLayer(hidden);

	return std::make_pair(actionProbs, actionValues);
}

void ActorCriticNetworkImpl::CopyParametersFrom(ActorCriticNetworkImpl source, float decay /* = 0 */)
{
	unsigned int i = 0;
	std::vector<torch::Tensor> sourceParameters = source.parameters();
	for (torch::Tensor localParameter : this->parameters())
	{
		localParameter.data().copy_(decay * localParameter.data() + (1 - decay) * sourceParameters[i].data());
		i++;
	}
}

void ActorCriticNetworkImpl::CopyGradientsFrom(ActorCriticNetworkImpl source)
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
		inputDims, hiddenLayerDims, actionLayerDims);
}
