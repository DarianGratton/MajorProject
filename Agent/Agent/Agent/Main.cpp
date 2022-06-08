#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <numeric>

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

	// ACERAgent test(0.0003f, 2, 5, 5, 32, 32, 1000000, 256)
	// cout << tensor2 << endl;

	AgentTest test(false, true);
	test.Train();

	return 0;
}
