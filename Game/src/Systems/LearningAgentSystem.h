#pragma once

#include <entityx/entityx.h>
#include <GameAgent/GameAgent.h>

#include <string>
#include <numeric>
#include <random>
#include <set>
#include <map>

#include "TrainingData.h"
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
        currState.AddDelta("PlayerPosX");
        currState.AddDelta("PlayerPosY");
        currState.AddDelta("PlayerHp");
        currState.AddDelta("PlayerWeapon1");
        currState.AddDelta("PlayerWeapon1PosX");
        currState.AddDelta("PlayerWeapon1PosY");
        currState.AddDelta("PlayerWeapon2");
        currState.AddDelta("PlayerWeapon2PosX");
        currState.AddDelta("PlayerWeapon2PosY");
        currState.AddDelta("EnemyPosX");
        currState.AddDelta("EnemyPosY");
        currState.AddDelta("EnemyHp");
        currState.AddDelta("EnemyWeapon1");
        currState.AddDelta("EnemyWeapon1PosX");
        currState.AddDelta("EnemyWeapon1PosY");
        currState.AddDelta("EnemyWeapon2");
        currState.AddDelta("EnemyWeapon2PosX");
        currState.AddDelta("EnemyWeapon2PosY");

        // Initialize Environment 
        env = std::make_shared<GameAgent::Environment>(currState);

        // Setup Network Parameters
        GameAgent::Networks::ACERParameters params;
        params.learningRate = 1e-3f;
        params.nActions = 1;
        params.nPossibleActions = 6;
        params.maxEpisodeLength = 1024;
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
        nEpisodes = 500;
        maxEpisodeLength = params.maxEpisodeLength;
        currMoveSetIndex = 0;

        // Initialize Agent
        agent = std::make_shared<GameAgent::Agent>(env, params);
        // agent->LoadAgent();
        agent->ClearStorage();

        // Subscribe to events
        events.subscribe<SceneLoad>(*this);
    }

    void update(EntityManager& es, EventManager& events, TimeDelta dt) override
    {
        // Only want to run when on arena scene
        if (!arenaLoaded || (Game::Instance().IsGamePaused() && gameEndedAndAgentSaved))
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

        // Set Player Action
        if (Game::Instance().IsAutomaticallyTraining() && !currPlayerMoveSet.empty())
        {
            if (currMoveSetIndex >= currPlayerMoveSet.size())
                currMoveSetIndex = 0;

            reinterpret_cast<PlayerScript*>(playerScript->script)->SetCharacterAction(currPlayerMoveSet[currMoveSetIndex]);
            currMoveSetIndex++;
        }

        // Train Agent
        if (Game::Instance().IsAgentTraining())
            agent->Train();

        // Game Ended and Agent is being Automatically Trained
        bool automaticTrainingEnded = Game::Instance().IsAutomaticallyTraining() && (terminal || env->GetSteps() >= maxEpisodeLength);
        if (automaticTrainingEnded)
        {
            // Calculate average reward
            float avgReward = 0;
            episodesRewardHistory.push_back(env->GetTotalReward());
            if (episodesRewardHistory.size() >= 100)
                avgReward = std::accumulate(episodesRewardHistory.end() - 100, episodesRewardHistory.end(), 0.0) / 100;
            else
                avgReward = std::accumulate(episodesRewardHistory.begin(), episodesRewardHistory.end(), 0.0) / episodesRewardHistory.size();

            // Check if agent won game
            if (currState.GetDelta("PlayerHp") <= 0)
            {
                winCount++;
            }

            // Print results
            std::cout << "Game: " << nEpisodeCount << "/" << nEpisodes << std::endl;
            std::cout << "Game's Won: " << winCount << "/" << nEpisodes << std::endl;
            std::cout << "Final Step Count: " << env->GetSteps() << std::endl;
            std::cout << "Final Reward: " << env->GetTotalReward() << std::endl;
            std::cout << "Past 100 Episodes AvgReward: " << avgReward << std::endl;

            // Save the Agent
            if (avgReward > bestAvgReward && episodesRewardHistory.size() >= 100)
            {
                bestAvgReward = avgReward;
                agent->SaveAgent();
            }
            std::cout << "Overall BestAvgReward: " << bestAvgReward << std::endl << std::endl;

            // Save Utility
            agent->SaveUtility();

            // Update Variables
            nEpisodeCount++;
            gameEndedAndAgentSaved = true;

            if (nEpisodeCount <= nEpisodes)
            {
                // Reload Scene
                SceneManager::Instance().LoadScene("Arena");
            }
            else
            {
                // Otherwise
                Game::Instance().PauseGame();
                LOG_INFO("Training of Agent Finished");
            }
        }

        // Game Ended and Agent is being Manually Trained
        bool manualTrainingEnded = Game::Instance().IsManuallyTraining() && terminal;
        if (manualTrainingEnded)
        {
            // Save and Update the Agent
            agent->SaveAgent();
            agent->SaveUtility();

            // Update Variables
            gameEndedAndAgentSaved = true;
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
        {
            arenaLoaded = true;
        }

        // Initialize Initial State
        UpdateCurrentState(sl.entities);

        // Set up Player
        if (Game::Instance().IsAutomaticallyTraining())
        {
            // Set Player Weapons Randomly
            auto pickedWeapons = TrainingData::GetWeaponCombination();
            PlayerPrefs::Instance().SetWeapon1(pickedWeapons.first);
            PlayerPrefs::Instance().SetWeapon2(pickedWeapons.second);
            PlayerPrefs::Instance().SetWeapon1(1);
            PlayerPrefs::Instance().SetWeapon2(2);
            
            // Get Player Moveset
            currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::NotMoving, false);

            // Find Player Script
            ComponentHandle<Script> entityScript;
            for (Entity entity : sl.entities->entities_with_components(entityScript))
            {
                ComponentHandle<Name> entityName = entity.component<Name>();
                if (entityName.get()->name == "Player")
                {
                    playerScript = entity.component<Script>();
                    break;
                }
            }

            // Take weapons and send them to the player script
            reinterpret_cast<PlayerScript*>(playerScript->script)->SetCharacterWeapons(
                PlayerPrefs::Instance().GetWeapon1(), 
                PlayerPrefs::Instance().GetWeapon2());

            // Set Player's first action
            if (!currPlayerMoveSet.empty())
            {
                reinterpret_cast<PlayerScript*>(playerScript->script)->SetCharacterAction(currPlayerMoveSet[currMoveSetIndex]);
                currMoveSetIndex++;
            }
        }
        currState.UpdateDelta("PlayerWeapon1", PlayerPrefs::Instance().GetWeapon1());
        currState.UpdateDelta("PlayerWeapon2", PlayerPrefs::Instance().GetWeapon2());

        // Set Delta Enemy Weapons
        int weapon1 = 1;
        int weapon2 = 2;
        if (Game::Instance().IsAutomaticallyTraining())
        {
            // Randomly pick weapons
            auto pickedWeapons = TrainingData::GetWeaponCombination();
            weapon1 = pickedWeapons.first;
            weapon2 = pickedWeapons.second;
            weapon1 = 1;
            weapon2 = 2;
        }
        else
        {
            GameAgent::State searchState;
            searchState.AddDelta("PlayerWeapon1", PlayerPrefs::Instance().GetWeapon1());
            searchState.AddDelta("PlayerWeapon2", PlayerPrefs::Instance().GetWeapon2());

            auto utilityStates = agent->SearchUtilityStorage(searchState);
            if (!utilityStates.empty())
            {
                float highestUtility = std::numeric_limits<float>::min();
                GameAgent::State highestUtilityState = utilityStates.at(0).first;
                for (auto stateUtilityPair : utilityStates)
                {
                    if (stateUtilityPair.second > highestUtility)
                    {
                        highestUtilityState = stateUtilityPair.first;
                        highestUtility = stateUtilityPair.second;
                    }
                }

                weapon1 = highestUtilityState.GetDelta("EnemyWeapon1");
                weapon2 = highestUtilityState.GetDelta("EnemyWeapon2");
            }
            else
            {
                auto pickedWeapons = TrainingData::GetWeaponCombination();
                weapon1 = pickedWeapons.first;
                weapon2 = pickedWeapons.second;
            }
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
        gameEndedAndAgentSaved = false;
        env->Reset();
        env->SetInitState(currState);
        Game::Instance().SetTerminalState(false);

        // Setup Training Variables
        gameEndedAndAgentSaved = false;

        // Predict agent's first action
        predictedAction = agent->PredictAction(currState);
        reinterpret_cast<EnemyScript*>(enemyScript->script)->SetCharacterAction(predictedAction.at(0));
    }

private:

    /* */
    void UpdateCurrentState(EntityManager* entities)
    {
        // Reset Specific Deltas
        currState.UpdateDelta("PlayerWeapon1PosX", 0.0f);
        currState.UpdateDelta("PlayerWeapon1PosY", 0.0f);
        currState.UpdateDelta("PlayerWeapon2PosX", 0.0f);
        currState.UpdateDelta("PlayerWeapon2PosY", 0.0f);
        currState.UpdateDelta("EnemyWeapon1PosX", 0.0f);
        currState.UpdateDelta("EnemyWeapon1PosY", 0.0f);
        currState.UpdateDelta("EnemyWeapon2PosX", 0.0f);
        currState.UpdateDelta("EnemyWeapon2PosY", 0.0f);

        // Update delta positions and hp
        ComponentHandle<Transform> entityTransform;
        ComponentHandle<Script> entityScript;
        for (Entity entity : entities->entities_with_components(entityTransform, entityScript))
        {
            ComponentHandle<Active> entityActive = entity.component<Active>();
            if (!entityActive->isActive)
                continue;

            ComponentHandle<Name> entityName = entity.component<Name>();
            if (entityName.get()->name.find("PlayerWeapon1_") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("PlayerWeapon1PosX", entityTransform->xpos);
                currState.UpdateDelta("PlayerWeapon1PosY", entityTransform->ypos);
                continue;
            }

            if (entityName.get()->name.find("PlayerWeapon2_") != std::string::npos)
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

            if (entityName.get()->name.find("EnemyWeapon1_") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("EnemyWeapon1PosX", entityTransform->xpos);
                currState.UpdateDelta("EnemyWeapon1PosY", entityTransform->ypos);
                continue;
            }

            if (entityName.get()->name.find("EnemyWeapon2_") != std::string::npos)
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
        float reward = -0.01f;

        // Player Loses Health
        if (prevState.GetDelta("PlayerHp") > currState.GetDelta("PlayerHp"))
            reward += 5.0f;

        // AI Loses Health
        if (prevState.GetDelta("EnemyHp") > currState.GetDelta("EnemyHp"))
            reward -= 5.0f;

        // Player Wins Game
        if (currState.GetDelta("EnemyHp") <= 0) 
            reward -= 50.0f;

        // AI Wins Game
        if (currState.GetDelta("PlayerHp") <= 0)
            reward += 50.0f;

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
    ComponentHandle<Script> playerScript;

    /* */
    std::vector<unsigned int> currPlayerMoveSet;

    /* */
    int currMoveSetIndex;

    /* */
    float bestAvgReward = 0;

    /* */
    std::vector<float> episodesRewardHistory;

    /* */
    unsigned int winCount = 0;

    /* */
    unsigned int nEpisodes = 0;

    /* */
    unsigned int maxEpisodeLength = 0;

    /* */
    unsigned int nEpisodeCount = 1;

    /* */
    bool arenaLoaded = false;

    /* TODO: Band-aid solution to Game being paused after a game and the agent still needs to save. */
    bool gameEndedAndAgentSaved = false;

};
