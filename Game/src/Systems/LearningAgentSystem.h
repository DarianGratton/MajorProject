#pragma once

#include <entityx/entityx.h>
#include <GameAgent/GameAgent.h>

#include <string>
#include <numeric>
#include <random>
#include <set>
#include <map>

#include "../SceneManager.h"
#include "../Game.h"
#include "../ECS.h"
#include "../Components.h"
#include "../PlayerPrefs.h"
#include "../Logger.h"

using namespace entityx;

class LearningAgentSystem : public System<LearningAgentSystem>, public Receiver<LearningAgentSystem>
{
public:

    void configure(EventManager& events) override
    {
        // Initialize Initial State (18 Deltas)
        // TODO: Add alt. function that just adds the name and initializes it to zero
        currState.AddDelta("PlayerPosX", 0.0f);
        currState.AddDelta("PlayerPosY", 0.0f);
        currState.AddDelta("PlayerHp", 0.0f);
        currState.AddDelta("PlayerWeapon1", 0.0f);
        currState.AddDelta("PlayerWeapon1PosX", 0.0f);
        currState.AddDelta("PlayerWeapon1PosY", 0.0f);
        currState.AddDelta("PlayerWeapon2", 0.0f);
        currState.AddDelta("PlayerWeapon2PosX", 0.0f);
        currState.AddDelta("PlayerWeapon2PosY", 0.0f);
        currState.AddDelta("EnemyPosX", 0.0f);
        currState.AddDelta("EnemyPosY", 0.0f);
        currState.AddDelta("EnemyHp", 0.0f);
        currState.AddDelta("EnemyWeapon1", 0.0f);
        currState.AddDelta("EnemyWeapon1PosX", 0.0f);
        currState.AddDelta("EnemyWeapon1PosY", 0.0f);
        currState.AddDelta("EnemyWeapon2", 0.0f);
        currState.AddDelta("EnemyWeapon2PosX", 0.0f);
        currState.AddDelta("EnemyWeapon2PosY", 0.0f);

        // Initialize Environment 
        env = std::make_shared<GameAgent::Environment>(currState);

        // Setup Network Parameters
        GameAgent::Networks::ACERParameters params;
        params.learningRate = 1e-3f;
        params.nActions = 1;
        params.nPossibleActions = 6;
        params.maxEpisodeLength = 1000;
        params.inputDims = currState.Size();
        params.hiddenLayerDims = 128;
        params.nHiddenLayers = 8;
        params.actionLayerDims = 256;
        params.memSize = 1000000;
        params.batchSize = 16;
        params.batchTrajectoryLength = 100;
        params.biasWeight = 0.1f;
        params.gamma = 0.99f;
        params.traceMax = 10;

        // Initialize System
        maxEpisodeLength = params.maxEpisodeLength;

        // Initialize Agent
        agent = std::make_shared<GameAgent::Agent>(env, params);

        // Subscribe to events
        events.subscribe<SceneLoad>(*this);
    }

    void update(EntityManager& es, EventManager& events, TimeDelta dt) override
    {
        // Only want to run when on arena scene
        if (!arenaLoaded || Game::Instance().IsGamePaused())
            return;

        // Update Current State
        UpdateCurrentState(&ECS::Instance().entities);

        // Get Reward
        float reward = CalculateReward();

        // Get Terminal
        float terminal = Game::Instance().IsTerminalState();

        // Update environment
        env->Update(predictedAction, reward, currState, terminal);

        // Predict next action
        predictedAction = agent->PredictAction(currState);
        reinterpret_cast<EnemyScript*>(enemyScript->script)->SetCharacterAction(predictedAction.at(0));

        // Train Agent
        if (Game::Instance().IsAgentTraining())
            agent->Train();

        // Terminal state reached or episode ended
        if (Game::Instance().IsAgentTraining() && 
            (terminal || env->GetSteps() >= maxEpisodeLength))
        {
            // Calculate average reward
            float avgReward = 0;
            std::vector<float> rewardHistory = env->GetRewardHistory();
            if (rewardHistory.size() >= 100)
                avgReward = std::accumulate(rewardHistory.end() - 100, rewardHistory.end(), 0.0) / 100;
            else
                avgReward = std::accumulate(rewardHistory.begin(), rewardHistory.end(), 0.0) / rewardHistory.size();

            // Check if agent won game
            if (currState.GetDeltas().at("EnemyHp") <= 0)
            {
                winCount++;
            }

            // Print results
            std::cout << "Game: " << nEpisodeCount << "/" << maxEpisodeLength << std::endl;
            std::cout << "Game's Won: " << winCount << "/" << maxEpisodeLength << std::endl;
            std::cout << "Final Step Count: " << env->GetSteps() << std::endl;
            std::cout << "Final Reward: " << env->GetTotalReward() << std::endl;
            std::cout << "Past 100 Episodes AvgReward: " << avgReward << std::endl;
            
            // Save and update the agent
            if (avgReward > bestAvgReward && rewardHistory.size() >= 100)
            {
                bestAvgReward = avgReward;
                agent->SaveAgent();
            }
            agent->SaveUtility();
            std::cout << "Overall BestAvgReward: " << bestAvgReward << std::endl << std::endl;

            // Update variables
            nEpisodeCount++;

            // Reload Scene
            SceneManager::Instance().LoadScene("Arena");
        }
    }

    void receive(const SceneLoad& sl)
    {
        if (sl.sceneName != "Arena") 
        {
            arenaLoaded = false;
            return;
        }
        else
            arenaLoaded = true;

        // Initialize Initial State
        UpdateCurrentState(sl.entities);
        currState.UpdateDelta("PlayerWeapon1", PlayerPrefs::Instance().GetWeapon1());
        currState.UpdateDelta("PlayerWeapon2", PlayerPrefs::Instance().GetWeapon2());

        // Set Delta Enemy Weapons
        bool training = false;
        int weapon1 = 0;
        int weapon2 = 0;
        if (!training)
            // Not Training
        {
            GameAgent::State searchState;
            searchState.AddDelta("PlayerWeapon1", PlayerPrefs::Instance().GetWeapon1());
            searchState.AddDelta("PlayerWeapon2", PlayerPrefs::Instance().GetWeapon2());

            auto utilityStates = agent->SearchUtilityStorage(searchState);
            if (!utilityStates.empty())
                // Storage returned states
            {
                float highestUtility = std::numeric_limits<float>::min();
                GameAgent::State highestUtilityState;
                for (auto stateUtilityPair : utilityStates)
                {
                    if (stateUtilityPair.second > highestUtility)
                    {
                        highestUtilityState = stateUtilityPair.first;
                        highestUtility = stateUtilityPair.second;
                    }
                }

                weapon1 = highestUtilityState.GetDeltas().at("EnemyWeapon1");
                weapon2 = highestUtilityState.GetDeltas().at("EnemyWeapon2");
            }
            else
                // Storage had no states
            {
                // TODO: Not random
                std::set<int> weapons;
                while (weapons.size() < 2)
                {
                    weapons.insert(rand() % 5);
                }

                weapon1 = (*std::next(weapons.begin(), 0)) + 1;
                weapon2 = (*std::next(weapons.begin(), 1)) + 1;
            }
        }
        else
            // Training
        {
            // TODO: Figure out how to do this. (Milestone 7)
        }

        // Set enemy script
        ComponentHandle<Script> entityScript;
        for (Entity entity : sl.entities->entities_with_components(entityScript))
        {
            ComponentHandle<Name> entityName = entity.component<Name>();
            if (entityName.get()->name == "Enemy")
                enemyScript = entity.component<Script>();
        }

        // Set deltas
        currState.UpdateDelta("EnemyWeapon1", weapon1);
        currState.UpdateDelta("EnemyWeapon2", weapon2);

        // Take weapons and send them to the enemy script
        reinterpret_cast<EnemyScript*>(enemyScript->script)->SetCharacterWeapons(weapon1, weapon2);

        // Setup environment
        env->Reset();
        env->SetInitState(currState);
        Game::Instance().SetTerminalState(false);

        // Predict agent's first action
        predictedAction = agent->PredictAction(currState);
        reinterpret_cast<EnemyScript*>(enemyScript->script)->SetCharacterAction(predictedAction.at(0));
    }

private:

    /* */
    void UpdateCurrentState(EntityManager* entities)
    {
        // Update delta positions and hp
        ComponentHandle<Transform> entityTransform;
        ComponentHandle<Script> entityScript;
        for (Entity entity : entities->entities_with_components(entityTransform, entityScript))
        {
            ComponentHandle<Name> entityName = entity.component<Name>();
            if (entityName.get()->name.find("PlayerWeapon1") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("PlayerWeapon1PosX", entityTransform->xpos);
                currState.UpdateDelta("PlayerWeapon1PosY", entityTransform->ypos);
                continue;
            }

            if (entityName.get()->name.find("PlayerWeapon2") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("PlayerWeapon2PosX", entityTransform->xpos);
                currState.UpdateDelta("PlayerWeapon2PosY", entityTransform->ypos);
                continue;
            }

            if (entityName.get()->name.find("Player") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("PlayerPosX", entityTransform->xpos);
                currState.UpdateDelta("PlayerPosY", entityTransform->ypos);

                entityScript = entity.component<Script>();
                float playerHealth = static_cast<float>(reinterpret_cast<PlayerScript*>(entityScript->script)->GetHealth());
                currState.UpdateDelta("PlayerHp", playerHealth);
                continue;
            }

            if (entityName.get()->name.find("EnemyWeapon1") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("EnemyWeapon1PosX", entityTransform->xpos);
                currState.UpdateDelta("EnemyWeapon1PosY", entityTransform->ypos);
                continue;
            }

            if (entityName.get()->name.find("EnemyWeapon2") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("EnemyWeapon2PosX", entityTransform->xpos);
                currState.UpdateDelta("EnemyWeapon2PosY", entityTransform->ypos);
                continue;
            }

            if (entityName.get()->name.find("Enemy") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("EnemyPosX", entityTransform->xpos);
                currState.UpdateDelta("EnemyPosY", entityTransform->ypos);

                entityScript = entity.component<Script>();
                float enemyHealth = static_cast<float>(reinterpret_cast<EnemyScript*>(entityScript->script)->GetHealth());
                currState.UpdateDelta("EnemyHp", enemyHealth);
                continue;
            }
        }
    }

    /* 
      Very basic reward calculation.
      NOTE: In Final Report talk about the alternatives to calculate rewards.
    */
    float CalculateReward()
    {
        GameAgent::State prevState = env->GetCurrState();
        float reward = 0.0f;

        // Player Loses Health
        if (prevState.GetDelta("PlayerHp") > currState.GetDelta("PlayerHp"))
            reward += 1.0f;

        // AI Loses Health
        if (prevState.GetDelta("EnemyHp") > currState.GetDelta("EnemyHp"))
            reward -= 1.0f;

        // Player Wins Game
        if (currState.GetDelta("EnemyHp") <= 0)
            reward -= 10.0f;

        // AI Wins Game
        if (currState.GetDelta("PlayerHp") <= 0)
            reward += 10.0f;

        return reward;
    }
    
    /* */
    std::shared_ptr<GameAgent::Agent> agent;
    
    /* */
    std::shared_ptr<GameAgent::Environment> env;

    /* */
    GameAgent::State currState;

    /* */
    ComponentHandle<Script> enemyScript;

    /* */
    std::vector<float> predictedAction;

    /* */
    float bestAvgReward = 0;

    /* */
    unsigned int winCount = 0;

    /* */
    unsigned int maxEpisodeLength = 0;

    /* */
    unsigned int nEpisodeCount = 1;

    /* */
    bool arenaLoaded = false;

};
