#include "AgentTest.h"

#include <iostream>
#include <fstream>
#include <Eigen/Core>
#include <torch/torch.h>
#include <random>
#include <vector>
#include <numeric>

#include "TestEnvironment.h"
#include "../../Environment/State.h"

using namespace RLGameAgent;

AgentTest::AgentTest()
{
    // Model.
    float lr1 = 1e-3f;
    float lr2 = 1e-3f;
    unsigned int nActions = 2;
    unsigned int maxActions = 2;
    int64_t inputDims = 4;
    int64_t layer1Dims = 16;
    int64_t layer2Dims = 32;
    unsigned int memSize = 1000000;
    float gamma = 0.99f;
    float tau = 0.005f;
    unsigned int batchSize = 1024;
    int rewardScale = 2;

    agent = new SACAgent(lr1, lr2, nActions, maxActions,
        inputDims, layer1Dims, layer2Dims,
        memSize, gamma, tau, batchSize, rewardScale);
}

void AgentTest::Train()
{
    // Random engine.
    std::random_device rd;
    std::mt19937 re(rd());
    std::uniform_int_distribution<> dist(-5, 5);

    // Environment.
    float x = float(dist(re)); // goal x pos
    float y = float(dist(re)); // goal y pos
    TestEnvironment env(x, y);

    // Model.
   /* float lr1 = 1e-3f;
    float lr2 = 1e-3f;
    unsigned int nActions = 2;
    unsigned int maxActions = 2; 
    int64_t inputDims = 4;
    int64_t layer1Dims = 16;
    int64_t layer2Dims = 32;
    unsigned int memSize = 1000000;
    float gamma = 0.99f;
    float tau = 0.005f;
    unsigned int batchSize = 1024;
    int rewardScale = 2;

    SACAgent agent(lr1, lr2, nActions, maxActions,
        inputDims, layer1Dims, layer2Dims,
        memSize, gamma, tau, batchSize, rewardScale);*/
    // TODO: He normalizes the network, unknown if relevent

    // Training loop.
    unsigned int nGames = 10000;
    unsigned int n_steps = 2048;

    // Output.
    std::ofstream out;
    out.open("Data/data.csv");

    // episode, agent_x, agent_y, goal_x, goal_y, STATUS=(PLAYING, WON, LOST, RESETTING)
    out << 1 << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << env.RESETTING << "\n";

    // Counter.
    unsigned int counter = 0;

    // Average reward.
    // TODO: Reward object
    float bestAvgReward = -100000; 
    float avgReward = 0;
    vector<float> rewardHistory;
    int winCount = 0;

    // REVIEW: From here down
    for (unsigned int episode = 1; episode <= nGames; episode++)
    {
        printf("Game %u/%u\n", episode, nGames);

        // State of Env.
        State observation;
        torch::Tensor state = env.State();
        observation.AddDelta(pair<string, float>("GoalX", x));
        observation.AddDelta(pair<string, float>("GoalY", y));
        observation.AddDelta(pair<string, float>("PosX", state[0][0].item<float>()));
        observation.AddDelta(pair<string, float>("PosY", state[0][1].item<float>()));

        float reward = 0;

        unsigned int i = 0;
        for (i = 1; i <= n_steps; i++)
        {
            // Play
            torch::Tensor action = agent->ChooseAction(observation);

            
            /*cout << "Action: " << endl;
            cout << action << endl;*/
            

            float xAct = action[0].item<float>();
            float yAct = action[1].item<float>();
            auto sd = env.Act(xAct, yAct); // std::tuple<state, status, terminal>

            // Get parameters
            State newObservation;
            torch::Tensor newState = std::get<0>(sd);
            newObservation.AddDelta(pair<string, float>("GoalX", x));
            newObservation.AddDelta(pair<string, float>("GoalY", y));
            newObservation.AddDelta(pair<string, float>("X", newState[0][0].item<float>()));
            newObservation.AddDelta(pair<string, float>("Y", newState[0][1].item<float>()));

            std::vector<float> actions = { xAct, yAct };
            float newReward = env.Reward(std::get<1>(sd))[0].item<float>();
            // bool terminal = (std::get<1>(sd) == TestEnvironment::STATUS::WON) ? true : false;
            bool terminal = std::get<2>(sd)[0].item<float>();

            // Update Agent
            reward += newReward;
            out << episode << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << std::get<1>(sd) << "\n";
            
            agent->UpdateMemory(observation, actions, newReward, newObservation, terminal);
            agent->Learn();
            observation = newObservation;

            // Check game status
            if (std::get<2>(sd)[0].item<float>())
            {
                if (std::get<1>(sd) == TestEnvironment::STATUS::WON)
                    winCount++;
                
                break;
            }
        }

        // Update agent
        // agent->Learn();

        // Reset game
        x = float(dist(re));
        y = float(dist(re));
        env.SetGoal(x, y);
        env.Reset();
        out << episode << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << env.RESETTING << "\n";

        // Get rewards
        rewardHistory.push_back(reward);

        if (rewardHistory.size() >= 100)
            avgReward = accumulate(rewardHistory.end() - 100, rewardHistory.end(), 0.0) / 100;
        else
            avgReward = accumulate(rewardHistory.begin(), rewardHistory.end(), 0.0) / rewardHistory.size();
        
        cout << "Step count: " << i << endl;
        printf("Game's Won %u/%u\n", winCount, nGames);
        cout << "Reward: " << reward << endl;
        cout << "AvgReward: " << avgReward << endl;

        if ((avgReward > bestAvgReward && rewardHistory.size() >= 100) || episode % 10000 == 0)
        {
            bestAvgReward = avgReward;
            agent->SaveModel();
        }

        cout << "BestAvgReward: " << bestAvgReward << endl << endl;
    }

    out.close();
}

void AgentTest::Test()
{
    // Random engine.
    std::random_device rd;
    std::mt19937 re(rd());
    std::uniform_int_distribution<> dist(-5, 5);

    // Environment.
    float x = float(dist(re)); // goal x pos
    float y = float(dist(re)); // goal y pos
    TestEnvironment env(x, y);

    // Model.
    /*float lr1 = 1e-3f;
    float lr2 = 1e-3f;
    unsigned int nActions = 2;
    unsigned int maxActions = 2;
    int64_t inputDims = 4;
    int64_t layer1Dims = 256;
    int64_t layer2Dims = 256;
    unsigned int memSize = 1000000;
    float gamma = 0.99f;
    float tau = 0.005f;
    unsigned int batchSize = 1024;
    int rewardScale = 2;

    SACAgent agent(lr1, lr2, nActions, maxActions,
        inputDims, layer1Dims, layer2Dims,
        memSize, gamma, tau, batchSize, rewardScale);*/

    // Load Model
    // agent.LoadModel();

    // Training loop.
    unsigned int nGames = 10000;
    unsigned int nSteps = 2048;

    // Output.
    std::ofstream out;
    out.open("Data/data.csv");

    // episode, agent_x, agent_y, goal_x, goal_y, STATUS=(PLAYING, WON, LOST, RESETTING)
    out << 1 << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << env.RESETTING << "\n";

    // Counter.
    unsigned int winCount = 0;

    for (unsigned int episode = 1; episode <= nGames; episode++)
    {
        printf("Game %u/%u\n", episode, nGames);

        // State of Env.
        State observation;
        torch::Tensor state = env.State();
        observation.AddDelta(pair<string, float>("GoalX", x));
        observation.AddDelta(pair<string, float>("GoalY", y));
        observation.AddDelta(pair<string, float>("PosX", state[0][0].item<float>()));
        observation.AddDelta(pair<string, float>("PosY", state[0][1].item<float>()));

        float reward = 0;

        for (unsigned int i = 0; i < nSteps; i++)
        {
            // Play
            torch::Tensor action = agent->ChooseAction(observation);
            float xAct = action[0].item<float>();
            float yAct = action[1].item<float>();
            auto sd = env.Act(xAct, yAct); // std::tuple<state, status, terminal>

            // Get parameters
            State newObservation;
            torch::Tensor newState = std::get<0>(sd);
            newObservation.AddDelta(pair<string, float>("GoalX", x));
            newObservation.AddDelta(pair<string, float>("GoalY", y));
            newObservation.AddDelta(pair<string, float>("X", newState[0][0].item<float>()));
            newObservation.AddDelta(pair<string, float>("Y", newState[0][1].item<float>()));

            float newReward = env.Reward(std::get<1>(sd))[0].item<float>();
            bool terminal = (std::get<1>(sd) == TestEnvironment::STATUS::WON) ? true : false;

            // Update Agent
            reward += newReward;
            out << 1 << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << std::get<1>(sd) << "\n";

            // Check game status
            if (std::get<2>(sd)[0].item<float>())
            {
                if (terminal)
                    winCount++;

                break;
            }
        }

        // Reset game
        x = float(dist(re));
        y = float(dist(re));
        env.SetGoal(x, y);
        env.Reset();
        out << episode << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << env.RESETTING << "\n";

        // Print results
        printf("Game's Won %u/%u\n", winCount, nGames);
        cout << endl;
    }

    out.close();
}
