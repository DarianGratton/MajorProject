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

// Used for testing
int main()
{
    /*GameAgent::Networks::ACERParameters params;
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

    std::cout << "Test" << std::endl;

    std::vector<float> X = { 1, 2, 3, 4, 5 };
    std::vector<float> Y = { 1, 2, 3, 4, 5 };

    torch::Tensor tensor1 = torch::tensor({ {2, 4}, {2, 3} });
    torch::Tensor tensor2 = torch::tensor({ {1, 5}, {5, 7} });
    torch::Tensor betterTogether = torch::cat({ tensor1, tensor2 }, 0);
    std::cout << betterTogether << std::endl;


    GameAgent::Visualizer visualizer;
    visualizer.PlotLine(X, Y, "LinePlotTest.png", "Stuff");
    visualizer.PlotBar(X, Y, "BarPlotTest.png");*/

    UtilityStorage storage("AgentStorage.txt");
    GameAgent::State state;
    state.AddDelta("PlayerWeapon1", 3);
    state.AddDelta("PlayerWeapon2", 5);
    //auto states = storage.Search(state);

    state.UpdateDelta("PlayerWeapon2", 4);

    std::cout << state << std::endl;

    auto states = storage.Search(state);

    std::cout << states.size() << std::endl;
    // std::cout << states.at(0).first << std::endl;
    //std::cout << states.at(0).second << std::endl;

    //AgentTest test(false, true);
	//test.Train();
	//test.Test();

	return 0;
}
