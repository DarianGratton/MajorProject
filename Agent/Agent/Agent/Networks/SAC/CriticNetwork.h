#pragma once

#include <torch/torch.h>

#include <sstream>

/*
  CriticNetwork

  A class that defines the Critic Network for the Soft-Actor Critic Algorithm.
  Determines if a action taken in any given state could be considered good or bad.

  Author: Darian G.
*/
class CriticNetworkImpl : public torch::nn::Module 
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
	CriticNetworkImpl(float lr, unsigned int nActions,
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	/*
	  Forward of the network, takes a state and action and predicts its value.
	  params:
			- state: The current state of the environment.
			- action: The action(s) taken in that state.
	  returns:
			- Predicted value of the action taken in that state.
	*/
	torch::Tensor Forward(torch::Tensor state, torch::Tensor action);

	/* Returns an reference to the checkpoint. Used for saving the model. */
	inline std::stringstream& GetCheckpoint() { return checkpoint; }

	/* Returns an reference to the optimizer. Used for teaching the network. */
	inline torch::optim::Adam* GetOptimizer() { return optimizer; }

private:

	/* Learning Rate. */
	float learningRate;

	/* Number of actions. */
	unsigned int numActions;

	/* Network Layers */
	torch::nn::Linear layer1 = nullptr;
	torch::nn::Linear layer2 = nullptr;
	torch::nn::Linear q = nullptr;
	torch::optim::Adam* optimizer;

	/* Checkpoints */
	std::stringstream checkpoint;
};
TORCH_MODULE(CriticNetwork);
