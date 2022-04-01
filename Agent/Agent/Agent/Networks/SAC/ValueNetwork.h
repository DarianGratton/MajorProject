#pragma once

#include <torch/torch.h>

#include <sstream>
#include <string>

/*
  ValueNetwork

  A class that defines the Value Network for the Soft-Actor Critic Algorithm.
  Determines if any given state can be considered good or not.

  Author: Darian G.
*/
class ValueNetworkImpl : public torch::nn::Module
{
public:

	/*
	  Construtor for initializing the network and its layers.
	  params:
			- lr: The learning rate of the network.
			- inputDims: Input dimensions (size of state).
			- layer1Dims: Layer 1 dimensions.
			- layer2Dims: Layer 2 dimensions.
	*/
	ValueNetworkImpl(float lr, 
		int64_t inputDims, int64_t layer1Dims, int64_t layer2Dims);

	/*
	  Forward of the network, takes a state and predicts its value.
	  params:
			- state: The current state of the environment.
	  returns:
			- Predicted value of the state.
	*/
	torch::Tensor Forward(torch::Tensor state);

	/* Returns an reference to the checkpoint. Used for saving the model. */
	inline std::stringstream& GetCheckpoint() { return checkpoint; }

	/* Returns an reference to the optimizer. Used for teaching the network. */
	inline torch::optim::Adam* GetOptimizer() { return optimizer;  }

private:

	/* Learning Rate. */
	float learningRate;

	/* Network Layers */
	torch::nn::Linear layer1 = nullptr;
	torch::nn::Linear layer2 = nullptr;
	torch::nn::Linear value = nullptr;
	torch::optim::Adam* optimizer;

	/* Checkpoints */
	std::stringstream checkpoint;
};
TORCH_MODULE(ValueNetwork);
