#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <numeric>
#include <string>

#include "GameAgent.h"
#include "Environment/State.h"

using namespace std;

// Used for testing
int main()
{
	State state;
	state.AddDelta("Test", 1);
	state.AddDelta("Test", 2);
	state.RemoveDelta("Test2");

	cout << state.Size() << endl;

	//AgentTest test(false, true);
	//test.Train();
	//test.Test();

	return 0;
}
