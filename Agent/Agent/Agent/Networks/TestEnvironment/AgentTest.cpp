 #include "AgentTest.h"

#include <iostream>
#include <fstream>
#include <Eigen/Core>
#include <torch/torch.h>
#include <random>
#include <vector>
#include <numeric>

#include "../ACER/Trajectory.h"
#include "../ACER/ACERParameters.h"
#include "TestEnvironment.h"
#include "../../Environment/State.h"

AgentTest::AgentTest(bool isContinous, bool recordOutput) : 
    isContinous(isContinous), recordOutput(recordOutput)
{
    // Setup Model.
    // ...
    ACERParameters params;
    params.learningRate = 1e-3f;
    params.nActions = 1;
    params.nPossibleActions = 4;
    params.inputDims = 4;
    params.hiddenLayerDims = 128;
    params.actionLayerDims = 256;
    params.memSize = 1000000;
    batchSize = 16;
    params.batchSize = batchSize;
    params.biasWeight = 0.1f;
    params.gamma = 0.99f;
    params.traceMax = 10;

    agent = std::make_unique<ACERAgent>(params);
}

void AgentTest::Train()
{
    // Setup Enviroment
    std::random_device rd;
    std::mt19937 re(rd());
    std::uniform_int_distribution<> dist(-5, 5);
    
    float x = float(dist(re)); // goal x pos
    float y = float(dist(re)); // goal y pos
    TestEnvironment env(x, y);

    // Setup Output.
    std::ofstream out;
    if (recordOutput)
    {
        out.open("Data/data.csv");
        out << 1 << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << env.RESETTING << "\n";
    }

    // Training loop.
    unsigned int nEpisodes = 2000;
    unsigned int maxEpisodeLength = 2048;
    unsigned int winCount = 0;
    float bestAvgReward = -100000; 
    float avgReward = 0;
    std::vector<float> rewardHistory;
    for (unsigned int episode = 1; episode <= nEpisodes; episode++)
    {
        printf("Game %u/%u\n", episode, nEpisodes);

        // State of Env.
        State observation;
        torch::Tensor state = env.State();
        observation.AddDelta("GoalX", x);
        observation.AddDelta("GoalY", y);
        observation.AddDelta("PosX", state[0][0].item<float>());
        observation.AddDelta("PosY", state[0][1].item<float>());

        // Create trajectory
        Trajectory trajectory(maxEpisodeLength, observation.Size(), 1, 4);

        //cout << observation << endl;
        std::vector<Trajectory> trajectories;
        float reward = 0;
        unsigned int i = 0;
        for (i = 1; i <= maxEpisodeLength; i++)
        {
            //Trajectory trajectory(1, observation.Size(), 1, 4);

            // Play
            auto predictedAction = agent->PredictAction(observation.ToTensor());

            float xAct = 0.0f;
            float yAct = 0.0f;
            switch(static_cast<int>(predictedAction.at(0)))
            {
            case 0:
                xAct = 0.5f;
                break;
            case 1:
                xAct = -0.5f;
                break;
            case 2:
                yAct = 0.5f;
                break;
            case 3:
                yAct = -0.5f;
                break;
            default:
                std::cout << "Something went wrong" << std::endl;
                break;
            }

            auto sd = env.Act(xAct, yAct); // std::tuple<state, status, terminal>

            // Get parameters
            State newObservation;
            torch::Tensor newState = std::get<0>(sd);
            newObservation.AddDelta("GoalX", x);
            newObservation.AddDelta("GoalY", y);
            newObservation.AddDelta("X", newState[0][0].item<float>());
            newObservation.AddDelta("Y", newState[0][1].item<float>());

            float newReward = env.Reward(std::get<1>(sd))[0].item<float>();
            //bool terminal = (std::get<1>(sd) == TestEnvironment::STATUS::WON) ? true : false;
            bool terminal = std::get<2>(sd)[0].item<float>();

            // Update Agent
            reward += newReward;

            if (recordOutput)
                out << episode << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << std::get<1>(sd) << "\n";
            
            agent->UpdateMemory(observation, predictedAction, newReward,
                                newObservation, terminal);

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
        //agent->Learn();

        // Reset game
        x = float(dist(re));
        y = float(dist(re));
        env.SetGoal(x, y);
        env.Reset();

        if (recordOutput)
            out << episode << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << env.RESETTING << "\n";

        // Get rewards
        rewardHistory.push_back(reward);

        if (rewardHistory.size() >= 100)
            avgReward = accumulate(rewardHistory.end() - 100, rewardHistory.end(), 0.0) / 100;
        else
            avgReward = accumulate(rewardHistory.begin(), rewardHistory.end(), 0.0) / rewardHistory.size();
        
        std::cout << "Step count: " << i << std::endl;
        printf("Game's Won %u/%u\n", winCount, nEpisodes);
        std::cout << "Reward: " << reward << std::endl;
        std::cout << "AvgReward: " << avgReward << std::endl;

        if ((avgReward > bestAvgReward && rewardHistory.size() >= 100))
        {
            bestAvgReward = avgReward;
            agent->SaveModel();
        }

        std::cout << "BestAvgReward: " << bestAvgReward << std::endl << std::endl;
    }

    out.close();
}

void AgentTest::Test()
{
    // Test Code
    // ...
    // Setup Enviroment
    std::random_device rd;
    std::mt19937 re(rd());
    std::uniform_int_distribution<> dist(-5, 5);

    float x = float(dist(re)); // goal x pos
    float y = float(dist(re)); // goal y pos
    TestEnvironment env(x, y);

    // Setup Output.
    std::ofstream out;
    if (recordOutput)
    {
        out.open("Data/data.csv");
        out << 1 << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << env.RESETTING << "\n";
    }

    // Load Agent
    agent->LoadModel();

    // Training loop.
    unsigned int nEpisodes = 1000;
    unsigned int maxEpisodeLength = 2048;
    unsigned int winCount = 0;
    for (unsigned int episode = 1; episode <= nEpisodes; episode++)
    {
        printf("Game %u/%u\n", episode, nEpisodes);

        // State of Env.
        State observation;
        torch::Tensor state = env.State();
        observation.AddDelta("GoalX", x);
        observation.AddDelta("GoalY", y);
        observation.AddDelta("PosX", state[0][0].item<float>());
        observation.AddDelta("PosY", state[0][1].item<float>());

        float reward = 0;
        unsigned int i = 0;
        for (i = 1; i <= maxEpisodeLength; i++)
        {
            // Play
            auto predictedAction = agent->PredictAction(observation.ToTensor());

            float xAct = 0.0f;
            float yAct = 0.0f;
            switch (static_cast<int>(predictedAction.at(0)))
            {
            case 0:
                xAct = 0.5f;
                break;
            case 1:
                xAct = -0.5f;
                break;
            case 2:
                yAct = 0.5f;
                break;
            case 3:
                yAct = -0.5f;
                break;
            default:
                std::cout << "Something went wrong" << std::endl;
                break;
            }

            auto sd = env.Act(xAct, yAct); // std::tuple<state, status, terminal>

            // Get parameters
            State newObservation;
            torch::Tensor newState = std::get<0>(sd);
            newObservation.AddDelta("GoalX", x);
            newObservation.AddDelta("GoalY", y);
            newObservation.AddDelta("X", newState[0][0].item<float>());
            newObservation.AddDelta("Y", newState[0][1].item<float>());

            // Update Agent
            float newReward = env.Reward(std::get<1>(sd))[0].item<float>();
            reward += newReward;

            if (recordOutput)
                out << episode << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << std::get<1>(sd) << "\n";

            observation = newObservation;

            // Check game status
            if (std::get<2>(sd)[0].item<float>())
            {
                if (std::get<1>(sd) == TestEnvironment::STATUS::WON)
                    winCount++;

                break;
            }
        }

        // Reset game
        x = float(dist(re));
        y = float(dist(re));
        env.SetGoal(x, y);
        env.Reset();

        if (recordOutput)
            out << episode << ", " << env.pos(0) << ", " << env.pos(1) << ", " << env.goal(0) << ", " << env.goal(1) << ", " << env.RESETTING << "\n";

        std::cout << "Step count: " << i << std::endl;
        printf("Game's Won %u/%u\n", winCount, nEpisodes);
    }

    out.close();
}
