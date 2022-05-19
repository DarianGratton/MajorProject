#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "Environment/Environment.h"
#include "Environment/State.h"
#include "Agent.h"
#include "Networks/SAC/SACAgent.h"
#include "PyTorchExpansion/TorchNormal.h"
#include "Networks/TestEnvironment/AgentTest.h"

#include "Networks/ACER/ACERAgent.h"

using namespace std;

// Used for testing
int main()
{
	/*sacagent agent(0.0003f, 0.0003, 1, 1,
			5, 256, 256,
			1000000, 0.99f, 0.005f, 256, 2);
	agent.savemodel();
	agent.loadmodel();*/

	/*RLGameAgent::State state;
	state.AddDelta(make_pair<string, float>("X", 12.0f));
	state.AddDelta(make_pair<string, float>("Y", 123.0f));
	state.AddDelta(make_pair<string, float>("Value", 12323.0f));

	cout << state.ToString() << endl;
	cout << state << endl;*/

	/*AgentTest test;
	test.Train();
	test.Test();*/

	// ACERAgent test(0.0003f, 2, 5, 5, 32, 32, 1000000, 256);

	torch::Tensor tensor1 = torch::tensor({ {1, 2}, {3, 4}, {5, 6} });
	torch::Tensor tensor2 = torch::tensor({ {0, 0}, {0, 0}, {0, 0}, {0, 0} });

	// tensor2.copy_(tensor1.slice(0, 0, 2));
	tensor1 = tensor1.index({ torch::indexing::Slice(1, torch::indexing::None), "..." });
	cout << tensor1 << endl;
	cout << tensor2.size(0) << endl;
	// cout << tensor2 << endl;

	return 0;
}
