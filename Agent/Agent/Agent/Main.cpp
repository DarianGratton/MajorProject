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
    GameAgent::Networks::ACERParameters params;
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

    GameAgent::State state;
    state.AddDelta("X", 12);
    state.AddDelta("Y", 10);

    std::shared_ptr<GameAgent::Environment> env = std::make_shared<GameAgent::Environment>(state);
    GameAgent::Agent agent(env, params);

    GameAgent::State state2;
    state2.AddDelta("X", 12);
    state2.AddDelta("Y", 10);
    state2.AddDelta("Z", 10);

    if (state == state2)
    {
        std::cout << "Equal" << std::endl;
    }
    else
    {
        std::cout << "Not Equal" << std::endl;
    }

	//AgentTest test(false, true);
	//test.Train();
	//test.Test();

	return 0;
}
