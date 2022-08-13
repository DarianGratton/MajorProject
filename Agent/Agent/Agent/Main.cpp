#include "GameAgent.h"

#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <numeric>
#include <string>
#include <memory>

#include "Storage/UtilityStorage.h"

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

    UtilityStorage storage;
    storage.Save(state, 480.0f);
    storage.Save(state, 980.0f);

    GameAgent::State state2;
    state2.AddDelta("X", 7);
    state2.AddDelta("Y", 10);
    storage.Save(state2, 1000.0f);

    auto data = storage.Load();
    std::cout << data.at(0).first << std::endl;
    std::cout << data.at(0).second << std::endl;

	//AgentTest test(false, true);
	//test.Train();
	//test.Test();

	return 0;
}
