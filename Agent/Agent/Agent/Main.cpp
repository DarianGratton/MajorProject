#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <numeric>
#include <string>

#include "GameAgent.h"
#include "Networks/TestEnvironment/AgentTest.h"
#include "Environment/State.h"

using namespace std;

// Used for testing
int main()
{
	State state;
	state.UpdateDelta("Test", 1);

	//AgentTest test(false, true);
	//test.Train();
	//test.Test();

	return 0;
}
