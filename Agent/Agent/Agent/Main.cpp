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
#include "Networks/ACER/ActorCriticNetwork.h"

#include "Networks/ACER/ACERAgent.h"

using namespace std;

// Used for testing
int main()
{
	AgentTest test(false, true);
	test.Train();
	test.Test();

	return 0;
}
