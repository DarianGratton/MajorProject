#pragma once

#include <torch/torch.h>

#include <string>
#include <sstream>
#include <memory>

/*
  ActorCriticNetwork

  ***Description***

  Author: Darian G.
*/
class ActorCriticNetworkImpl : public torch::nn::Module
{
public:

	/*
	  Constructor for initializing the network and its layers.
	  params:
			- lr: The learning rate of the network.
			- nActions: Number of actions the agent takes in any given state.
			- inputDims: Input dimensions (size of state).
			- layer1Dims: Layer 1 dimensions.
			- layer2Dims: Layer 2 dimensions.
	*/
	ActorCriticNetworkImpl(float lr, unsigned int nPossibleActions,
		int64_t inputDims, int64_t hiddenLayerDims, int64_t actionLayerDims);

	/*
	  Forward pass of the network.
	  params:
			- state: The current state of the environment.
	  returns:
			- Predicted value and policy [description].
	*/
	std::pair<torch::Tensor, torch::Tensor> Forward(torch::Tensor state);

	/* TODO: source should be constant */
	void CopyParametersFrom(const ActorCriticNetworkImpl& source, float decay = 0);

	/* TODO: source should be constant */
	void CopyGradientsFrom(const ActorCriticNetworkImpl& source);

	/* 
	  Creates a new ActorCriticNetwork object using the parameters of the original 
	  object. Used to create a clone with refreshed hyperparameters and gradients.
	  returns:
			- A new ActorCriticNetwork object;
	*/
	std::shared_ptr<ActorCriticNetworkImpl> CleanClone();

	inline std::stringstream& GetCheckpoint() { return checkpoint; }
	inline torch::optim::Adam* GetOptimizer() { return optimizer; }

private:

	float learningRate;
	unsigned int nPossibleActions;
	int64_t inputDims;
	int64_t hiddenLayerDims;
	int64_t actionLayerDims;

	/* Network Layers */
	torch::nn::Linear inputLayer = nullptr;
	torch::nn::Linear hiddenLayer1 = nullptr;
	torch::nn::Linear hiddenLayer2 = nullptr;
	torch::nn::Linear hiddenLayer3 = nullptr;
	torch::nn::Linear actionLayer = nullptr;
	torch::nn::Linear actionValueLayer = nullptr;

	/* Optimizer */
	torch::optim::Adam* optimizer;

	/* Checkpoint */
	std::stringstream checkpoint;
};
TORCH_MODULE(ActorCriticNetwork);
