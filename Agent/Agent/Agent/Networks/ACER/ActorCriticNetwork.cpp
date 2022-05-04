#include "ActorCriticNetwork.h"

ActorCriticNetworkImpl::ActorCriticNetworkImpl(
	float lr, unsigned int nActions, 
	int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims) : 
	learningRate(lr), nActions(nActions),
	inputDims(inputDims), hiddenLayerDims(hiddenLayerDims), actionLayerDims(actionLayerDims),
	inputLayer(torch::nn::Linear(inputDims, hiddenLayerDims)),
	hiddenLayer1(torch::nn::Linear(hiddenLayerDims, actionLayerDims)),
	actionLayer(torch::nn::Linear(actionLayerDims, nActions)),
	actionValueLayer(torch::nn::Linear(actionLayerDims, nActions))
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

	torch::Tensor actionProbs = torch::nn::functional::softmax(actionLayer(hidden), 1);
	torch::Tensor actionValues = actionValueLayer(hidden);

	return std::make_pair(actionProbs, actionValues);
}

void ActorCriticNetworkImpl::ResetParameters()
{
	inputLayer->reset_parameters();
	hiddenLayer1->reset_parameters();
	actionLayer->reset_parameters();
	actionValueLayer->reset_parameters();
}

std::shared_ptr<ActorCriticNetworkImpl> ActorCriticNetworkImpl::CustomClone()
{
	auto newNetwork = std::make_shared<ActorCriticNetworkImpl>(
		                 learningRate, nActions, 
		                 inputDims, hiddenLayerDims, actionLayerDims);
	std::string data;
	{
		std::ostringstream oss;
		torch::serialize::OutputArchive archive;

		this->save(archive);
		archive.save_to(oss);
		data = oss.str();
	}

	{
		std::istringstream iss(data);
		torch::serialize::InputArchive archive;
		archive.load_from(iss);
		newNetwork->load(archive);
	}

	return newNetwork;
}
