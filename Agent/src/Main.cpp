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
#include "GameAgent.h"
#include "Networks/TestEnvironment/AgentTest.h"
#include "Visualizer/Visualizer.h"

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
    state.AddDelta("X Y", 12);
    state.AddDelta("Y Z", 10);

    std::shared_ptr<GameAgent::Environment> env = std::make_shared<GameAgent::Environment>(state);
    GameAgent::Agent agent(env, params);

    agent.LoadAgent();

    std::cout << "Test" << std::endl;

    std::vector<float> X = { 1, 2, 3, 4 };
    std::vector<float> Y = { 1, 2, 3, 4 };
    Visualizer visualizer;
    visualizer.PlotLine(X, Y, "LinePlotTest.png", "Stuff");
    visualizer.PlotBar(X, Y, "BarPlotTest.png");

   /* UtilityStorage storage;
    storage.Clear();
    storage.Save(state, 500.0f);
    storage.Save(state, 1000.0f);

    GameAgent::State state2;
    state2.AddDelta("X", 7);
    state2.AddDelta("Y", 10);
    state2.AddDelta("Z 1", 9);
    storage.Save(state2, 250.0f);

    GameAgent::State state3;
    state3.AddDelta("X Y", 12);
    state3.AddDelta("Y Z", 10);
    auto states = storage.Search(state3);

    std::cout << states.size() << std::endl;
    std::cout << states.at(0).first << std::endl;
    std::cout << states.at(0).second << std::endl; */

	// AgentTest test(false, true);
	// test.Train();
	// test.Test();

	return 0;
}
