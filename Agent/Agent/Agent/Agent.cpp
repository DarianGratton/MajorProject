// Agent.cpp : Defines the entry point for the application.
//

#include "Agent.h"

#include <torch/torch.h>
#include <iostream>

using namespace std;

// TODO: Implement functionality for a basic Policy Gradient network
class PolicyGradientNetwork : torch::nn::Module {
public:
	/// <summary>
	/// Constructs the object and creates the neutral network.
	/// </summary>
	/// <param name="N"></param>
	/// <param name="M"></param>
	PolicyGradientNetwork(int64_t N, int64_t M) 
	{
		/*W = register_parameter("W", torch::randn({ N, M }));
		b = register_parameter("b", torch::randn(M));*/
	}
	
	/// <summary>
	///	Model prediction that takes an input Tensor and returns an output as
	/// an Tensor of probabilities.
	/// </summary>
	/// <param name="input">Data to predict on</param>
	/// <returns>A Tensor of probability distributions</returns>
	torch::Tensor Forward(torch::Tensor input) 
	{
		return torch::addmm(b, input, W);
	}

	/// <summary>
	/// Updates the policy of the Agent's network.
	/// </summary>
	void UpdatePoicy() {}

private:
	torch::Tensor W, b;
};

class Agent {
public:
	Agent() {}

	/// <summary>
	///	Takes the current environment and returns an action.
	/// </summary>
	/// <returns></returns>
	std::vector<long> predictAction();

	/// <summary>
	/// TODO: parameters for training the network?
	/// </summary>
	void trainNetwork();

	void loadNetwork();
	void saveNetwork();
private:
	std::shared_ptr<PolicyGradientNetwork> model;
};
