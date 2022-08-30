#pragma once

#include <torch/torch.h>

#include <string>
#include <sstream>
#include <memory>

/*
  ActorCriticNetwork

  A class that defines the Actor-Critic Network of the ACER Algorithm.

  Author: Darian G.
*/
class ActorCriticNetworkImpl : public torch::nn::Module
{
public:

	/*
	  Constructor for initializing the network and its layers.
	  params:
			- lr: The learning rate of the network.
			- nPossibleActions: Number of actions the agent takes in any given state.
			- inputDims: Input dimensions (size of a state in a environment).
			- nHiddenLayers: Number of hidden layers.
			- hiddenLayerDims: Size of each hidden layer.
			- actionLayerDims: Size of the action layer.
	*/
	ActorCriticNetworkImpl(float lr, unsigned int nPossibleActions,
		int64_t inputDims, unsigned int nHiddenLayers, 
		int64_t hiddenLayerDims, int64_t actionLayerDims);

	/*
	  Forward pass of the network.
	  params:
			- state: State of the environment.
	  returns:
			- Predicted policy and value given the state.
	*/
	std::pair<torch::Tensor, torch::Tensor> Forward(torch::Tensor state);

	/*
	  Copys the parameters from an existing ActorCriticNetwork object.
	  params:
			- source: The ActorCriticNetowrk object to copy from.
			- decay: A weight to determine how much of the network's parameters is copied (0 = full copy).
	*/
	void CopyParametersFrom(const ActorCriticNetworkImpl& source, float decay = 0);

	/* 
	  Copys the gradients from an existing ActorCriticNetwork object.
	  params:
			- source: The ActorCriticNetowrk object to copy from.
	*/
	void CopyGradientsFrom(const ActorCriticNetworkImpl& source);

	/* 
	  Creates a new ActorCriticNetwork object using the parameters of the original 
	  object. Used to create a clone with refreshed hyperparameters and gradients.
	  returns:
			- A newly allocated ActorCriticNetwork object;
	*/
	std::shared_ptr<ActorCriticNetworkImpl> CleanClone();

	/* 
	  Gets an reference to the checkpoint. Used for saving and loading the model.
	*/
	inline std::stringstream& GetCheckpoint() { return checkpoint; }
	
	/* 
	  Gets an reference to the optimizer. Used for updating the network's parameters.
	*/
	inline torch::optim::Adam* GetOptimizer() { return optimizer; }

private:

	/* Learning rate of the network. */
	float learningRate;

	/* Number of actions the agent takes in any given state. */
	unsigned int nPossibleActions;

	/* Input dimensions (size of a state in a environment). */
	int64_t inputDims;

	/* Number of hidden layers. */
	int64_t hiddenLayerDims;

	/* Size of each hidden layer. */
	unsigned int nHiddenLayers;

	/* Size of the action layer. */
	int64_t actionLayerDims;

	/* Network Layers */
	torch::nn::Linear inputLayer = nullptr;
	torch::nn::Linear actionLayer = nullptr;
	torch::nn::Linear actionValueLayer = nullptr;
	std::vector<torch::nn::Linear> hiddenLayers;

	/* Optimizer */
	torch::optim::Adam* optimizer;

	/* Checkpoint */
	std::stringstream checkpoint;

};
TORCH_MODULE(ActorCriticNetwork);
