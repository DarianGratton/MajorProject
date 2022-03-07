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

using namespace std;

// Used for testing
int main()
{
	AgentTest test;
	test.Train();

	/*sacagent agent(0.0003f, 0.0003, 1, 1,
			5, 256, 256,
			1000000, 0.99f, 0.005f, 256, 2);
	agent.savemodel();
	agent.loadmodel();*/

	return 0;
}
