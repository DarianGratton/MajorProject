#pragma once

#include "../NetworkParameters.h"

namespace GameAgent
{
namespace Networks
{

/*
  ACERParameters

  Defines the parameters used to initialize a new ACERAgent object.

  Author: Darian G.
*/
class ACERParameters : public NetworkParameters
{
public: 

	/*
	  Default Constructor.
	*/
	ACERParameters() = default;

	/*
	  Gets the name of the network.
	*/
	inline std::string GetNetworkName() const override
	{
		return "ACER";
	}

	/* The learning rate of the agent. */
	float learningRate = 1e-3f;

	/* Number of actions the agent takes in any given state. */
	unsigned int nActions = 1;

	/* Number of actions the agent could take in any given state. */
	unsigned int nPossibleActions = 2;

	/* Input dimensions (size of a state in a environment). */
	int64_t inputDims = 4;

	/* Number of hidden layers. */
	unsigned int nHiddenLayers = 1;
	
	/* Size of each hidden layer. */
	int64_t hiddenLayerDims = 256;
	
	/* Size of the action layer. */
	int64_t actionLayerDims = 256;
	
	/* Physical size of the memory. */
	unsigned int memSize = 100000;
	
	/* Max length for a single episode in a environment. */
	unsigned int maxEpisodeLength = 256;
	
	/* Size of the batch sampled from memory. */
	unsigned int batchSize = 16;
	
	/* Length of each Trajectory that is sampled from memory. */
	unsigned int batchTrajectoryLength = 16;
	
	/* Actor loss weight. */
	float biasWeight = 0.1f;
	
	/* Reward discount factor. */
	float gamma = 0.99f;
	
	/* The trucation parameter. */
	int traceMax = 10;
	
	/* Constraint of the Trust Region Optimization. */
	float trustRegionConstraint = 1.0f;
	
	/* Decay of the Trust Region Optimization. */
	float trustRegionDecay = 0.99f;
};

} /* namespace Networks */
} /* namespace GameAgent */
