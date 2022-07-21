#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <numeric>
#include <string>

#include "GameAgent.h"
#include "Networks/TestEnvironment/AgentTest.h"

using namespace std;

// Used for testing
int main()
{
	AgentTest test(false, true);
	test.Train();
	test.Test();

	return 0;
}
