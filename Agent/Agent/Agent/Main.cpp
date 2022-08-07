#include "GameAgent.h"

#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <numeric>
#include <string>
#include <memory>

// Used for testing
int main()
{
    ACERParameters params;
    params.learningRate = 1e-3f;
    params.nActions = 1;
    params.nPossibleActions = 4;
    params.inputDims = 4;
    params.hiddenLayerDims = 128;
    params.actionLayerDims = 256;
    params.memSize = 1000000;
    params.batchSize = 16;
    params.biasWeight = 0.1f;
    params.gamma = 0.99f;
    params.traceMax = 10;

    State state;
    state.AddDelta("X", 12);
    state.AddDelta("Y", 10);

    std::shared_ptr<Environment> env = std::make_shared<Environment>(state);
    Agent agent(env, params);

	//AgentTest test(false, true);
	//test.Train();
	//test.Test();

	return 0;
}
