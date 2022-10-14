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

/* 
  LearningAgentSystem

  A class which handles all the logic of agent (AI) in the game environment.

  It creates the agent and it's environment, setups the agent when the scene loads, 
  calls the agent's PredictAction function to sent to the enemy script, handles training 
  when the agent is training in the environment and changes various functions when 
  the mode of training changes (modes include automatic training, manual training, and no training). 

  Author: Darian G.
*/
class LearningAgentSystem : public System<LearningAgentSystem>, public Receiver<LearningAgentSystem>
{
public:

    void configure(EventManager& events) override
    {
        // Initialize Initial State (28 Deltas)
        currState.AddDelta("PlayerPosX");
        currState.AddDelta("PlayerPosY");
        currState.AddDelta("PlayerDirection");
        currState.AddDelta("PlayerHp");
        currState.AddDelta("PlayerWeapon1");
        currState.AddDelta("PlayerWeapon1PosX");
        currState.AddDelta("PlayerWeapon1PosY");
        currState.AddDelta("PlayerWeapon1Active");
        currState.AddDelta("PlayerWeapon2");
        currState.AddDelta("PlayerWeapon2PosX");
        currState.AddDelta("PlayerWeapon2PosY");
        currState.AddDelta("PlayerWeapon2Active");
        currState.AddDelta("EnemyPosX");
        currState.AddDelta("EnemyPosY");
        currState.AddDelta("EnemyDirection");
        currState.AddDelta("EnemyHp");
        currState.AddDelta("EnemyWeapon1");
        currState.AddDelta("EnemyWeapon1PosX");
        currState.AddDelta("EnemyWeapon1PosY");
        currState.AddDelta("EnemyWeapon1Active");
        currState.AddDelta("EnemyWeapon2");
        currState.AddDelta("EnemyWeapon2PosX");
        currState.AddDelta("EnemyWeapon2PosY");
        currState.AddDelta("EnemyWeapon2Active");
        currState.AddDelta("EnemyDestinationPosX");
        currState.AddDelta("EnemyDestinationPosY");
        currState.AddDelta("EnemyDistanceToDestination");
        currState.AddDelta("EnemyDistanceToPlayer");

        // Initialize Environment 
        env = std::make_shared<GameAgent::Environment>(currState);

        // Setup Network Parameters
        GameAgent::Networks::ACERParameters params;
        params.learningRate = 1e-4f;
        params.nActions = 1;
        params.nPossibleActions = 6;
        params.maxEpisodeLength = 3072;
        params.inputDims = currState.Size();
        params.hiddenLayerDims = 14;
        params.nHiddenLayers = 5;
        params.actionLayerDims = 14;
        params.memSize = 10000000; 
        params.batchSize = 64;
        params.batchTrajectoryLength = 512;
        params.actorLossWeight = 0.1f;
        params.gamma = 0.99f;
        params.truncationParameter = 32;

        // Initialize System
        nEpisodes = 1500;
        maxEpisodeLength = params.maxEpisodeLength;
        currMoveSetIndex = 0;

        // Initialize Agent
        agent = std::make_shared<GameAgent::Agent>(env, params); 
        // agent->LoadAgent();
        agent->ClearStorage();

        // Setup Data Manipulator
        GameAgent::DataManip::Normalize normalize;
        agent->SetDataManipulation(std::make_shared<GameAgent::DataManip::Normalize>(normalize));

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
        bool terminal = Game::Instance().IsTerminalState();

        // Update environment
        env->Update(predictedAction, reward, currState, terminal);

        // Predict next action
        predictedAction = agent->PredictAction(currState);
        reinterpret_cast<EnemyScript*>(enemyScript->script)->SetCharacterAction(predictedAction[0]);

        // Set Player Action
        if (Game::Instance().IsAutomaticallyTraining() && !currPlayerMoveSet.empty())
        {
            if (currMoveSetIndex >= currPlayerMoveSet.size())
                currMoveSetIndex = 0;

            reinterpret_cast<PlayerScript*>(playerScript->script)->SetCharacterAction(currPlayerMoveSet[currMoveSetIndex]);
            currMoveSetIndex++;
        }
        // If no current player move set is selected then pick a random action for player
        else if (Game::Instance().IsAutomaticallyTraining())
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, 6);

            unsigned int action = dist(gen);
            reinterpret_cast<PlayerScript*>(playerScript->script)->SetCharacterAction(action);
        }

        // Train Agent
        if (Game::Instance().IsAgentTraining())
        {
            agent->Train();
        }

        // Game Ended and Agent is being Automatically Trained
        bool automaticTrainingEnded = Game::Instance().IsAutomaticallyTraining() && (terminal || env->GetSteps() >= maxEpisodeLength);
        if (automaticTrainingEnded)
        {
            // Calculate average reward
            float avgReward = 0;
            episodesRewardHistory.push_back(env->GetTotalReward());
            weaponsRewardHistory.at(currWeaponKey).push_back(env->GetTotalReward());
            if (episodesRewardHistory.size() >= 50)
                avgReward = std::accumulate(episodesRewardHistory.end() - 50, episodesRewardHistory.end(), 0.0) / 50;
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
            std::cout << "Past 50 Episodes AvgReward: " << avgReward << std::endl;

            // Save the Agent
            if (avgReward > bestAvgReward && episodesRewardHistory.size() >= 50)
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
                VisualizeWeaponRewards();
                VisualizeWeaponUtilities();
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
            // Set Player Weapons
            auto possibleWeapons = TrainingData::GetWeaponComps();
            auto pickedWeapons = possibleWeapons.at(currPlayerWeaponSetIndex);
            PlayerPrefs::Instance().SetWeapon1(pickedWeapons[0]);
            PlayerPrefs::Instance().SetWeapon2(pickedWeapons[1]);

            // Change weapons every n episodes
            if (nEpisodeCount % 10 == 0)
            {
                currPlayerWeaponSetIndex = (currPlayerWeaponSetIndex + 1 < possibleWeapons.size()) ? currPlayerWeaponSetIndex + 1 : 0;
            }

            // Used for training scenario's 1, 2, 3, and 4
            // PlayerPrefs::Instance().SetWeapon1(1);
            // PlayerPrefs::Instance().SetWeapon2(2); 
            
            // Get Player Moveset
            // Used for scenario 1 and 2
            // currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::NotMoving, false);

            // Used for scenario 3, 4, 5 and 6
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, 6);

            unsigned int move = dist(gen);
            bool isAttacking = true;
            switch (move)
            {
            case 0: 
                currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::NotMoving, isAttacking);
                currPlayerMoveSet = {}; // Random
                break;
            case 1:
                currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::MovingHorizontally, isAttacking);
                currPlayerMoveSet = {}; // Random
                break;
            case 2:
                currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::MovingVertically, isAttacking);
                currPlayerMoveSet = {}; // Random
                break;
            case 3:
                currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::MovingDiagonallyTopRight, isAttacking);
                currPlayerMoveSet = {}; // Random
                break;
            case 4:
                currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::MovingDiagonallyTopLeftToBottomRight, isAttacking);
                break;
            case 5:
                currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::MovingDiagonallyTopRight, isAttacking);
                break;
            case 6:
                currPlayerMoveSet = TrainingData::GetCommonMove(TrainingData::CommonMoves::MovingDiagonallyTopLeft, isAttacking);
                break;
            }

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
        
        // Update the state with the player's weapons
        currState.UpdateDelta("PlayerWeapon1", PlayerPrefs::Instance().GetWeapon1());
        currState.UpdateDelta("PlayerWeapon2", PlayerPrefs::Instance().GetWeapon2());

        // Set Delta Enemy Weapons
        int weapon1 = 1;
        int weapon2 = 2;
        if (Game::Instance().IsAutomaticallyTraining())
        {
            // Randomly pick weapons
            auto pickedWeapons = TrainingData::GetRandWeaponCombination();
            weapon1 = pickedWeapons.first;
            weapon2 = pickedWeapons.second;

            // Used for training scenario's 1, 2, and 3
            // weapon1 = 1;
            // weapon2 = 2;
        }
        else
        {
            // Create search state
            GameAgent::State searchState;
            searchState.AddDelta("PlayerWeapon1", PlayerPrefs::Instance().GetWeapon1());
            searchState.AddDelta("PlayerWeapon2", PlayerPrefs::Instance().GetWeapon2());

            // Get weapons from storage
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
            // No weapons in storage found
            else
            {
                auto pickedWeapons = TrainingData::GetRandWeaponCombination();
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

        // Setup weapon's reward history for visualization
        currWeaponKey = std::to_string(weapon1) + std::to_string(weapon2);
        if (weaponsRewardHistory.find(currWeaponKey) == weaponsRewardHistory.end())
        {
            weaponsRewardHistory.insert(std::make_pair(currWeaponKey, std::vector<float>()));
        }

        // Reset destination deltas to ensure that initial state isn't affected by them
        currState.UpdateDelta("EnemyDestinationPosX", 0.0f);
        currState.UpdateDelta("EnemyDestinationPosY", 0.0f);
        currState.UpdateDelta("EnemyDistanceToDestination", 0.0f);

        // Setup environment
        gameEndedAndAgentSaved = false;
        env->Reset();
        env->SetInitState(currState);
        Game::Instance().SetTerminalState(false);

        // Setup Training Variables
        gameEndedAndAgentSaved = false;

        // Predict agent's first action
        predictedAction = agent->PredictAction(currState);
        reinterpret_cast<EnemyScript*>(enemyScript->script)->SetCharacterAction(predictedAction[0]);
    }

private:

    /* 
      Updates the current state of the environment based on the entities in the scene.
      params:
            - entities: The entities of the current scene.
    */
    void UpdateCurrentState(EntityManager* entities)
    {
        // Reset Specific Deltas
        currState.UpdateDelta("PlayerWeapon1PosX", 0.0f);
        currState.UpdateDelta("PlayerWeapon1PosY", 0.0f);
        currState.UpdateDelta("PlayerWeapon1Active", 0.0f);
        currState.UpdateDelta("PlayerWeapon2PosX", 0.0f);
        currState.UpdateDelta("PlayerWeapon2PosY", 0.0f);
        currState.UpdateDelta("PlayerWeapon2Active", 0.0f);
        currState.UpdateDelta("EnemyWeapon1PosX", 0.0f);
        currState.UpdateDelta("EnemyWeapon1PosY", 0.0f);
        currState.UpdateDelta("EnemyWeapon1Active", 0.0f);
        currState.UpdateDelta("EnemyWeapon2PosX", 0.0f);
        currState.UpdateDelta("EnemyWeapon2PosY", 0.0f);
        currState.UpdateDelta("EnemyWeapon2Active", 0.0f);

        // Update delta positions and hp
        ComponentHandle<Transform> entityTransform;
        ComponentHandle<Script> entityScript;
        ComponentHandle<TextureComp> entityTexture;
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
                currState.UpdateDelta("PlayerWeapon1Active", 1.0f);
                continue;
            }

            if (entityName.get()->name.find("PlayerWeapon2_") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("PlayerWeapon2PosX", entityTransform->xpos);
                currState.UpdateDelta("PlayerWeapon2PosY", entityTransform->ypos);
                currState.UpdateDelta("PlayerWeapon2Active", 1.0f);
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

                entityTexture = entity.component<TextureComp>();
                if (entityTexture.get()->filename.find("Up") != std::string::npos)
                    currState.UpdateDelta("PlayerDirection", 1);

                else if (entityTexture.get()->filename.find("Down") != std::string::npos)
                    currState.UpdateDelta("PlayerDirection", 2);

                else if (entityTexture.get()->filename.find("Right") != std::string::npos)
                    currState.UpdateDelta("PlayerDirection", 3);

                else if (entityTexture.get()->filename.find("Left") != std::string::npos)
                    currState.UpdateDelta("PlayerDirection", 4);

                continue;
            }

            if (entityName.get()->name.find("EnemyWeapon1_") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("EnemyWeapon1PosX", entityTransform->xpos);
                currState.UpdateDelta("EnemyWeapon1PosY", entityTransform->ypos);
                currState.UpdateDelta("EnemyWeapon1Active", 1.0f);
                continue;
            }

            if (entityName.get()->name.find("EnemyWeapon2_") != std::string::npos)
            {
                entityTransform = entity.component<Transform>();
                currState.UpdateDelta("EnemyWeapon2PosX", entityTransform->xpos);
                currState.UpdateDelta("EnemyWeapon2PosY", entityTransform->ypos);
                currState.UpdateDelta("EnemyWeapon2Active", 1.0f);
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

                entityTexture = entity.component<TextureComp>();
                if (entityTexture.get()->filename.find("Up") != std::string::npos)
                    currState.UpdateDelta("EnemyDirection", 1);

                else if (entityTexture.get()->filename.find("Down") != std::string::npos)
                    currState.UpdateDelta("EnemyDirection", 2);

                else if (entityTexture.get()->filename.find("Right") != std::string::npos)
                    currState.UpdateDelta("EnemyDirection", 3);

                else if (entityTexture.get()->filename.find("Left") != std::string::npos)
                    currState.UpdateDelta("EnemyDirection", 4);

                continue;
            }
        }

        // Update distance from player
        float x = currState.GetDelta("EnemyPosX") - currState.GetDelta("PlayerPosX");
        float y = currState.GetDelta("EnemyPosY") - currState.GetDelta("PlayerPosY");
        float distance = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
        currState.UpdateDelta("EnemyDistanceToPlayer", distance);

        // Update Agent's Destination
        UpdateAgentDestination();
    }

    /* 
      Updates the state's enenmy destination delta based on the positon of the player, destination, and enemy.
    */
    void UpdateAgentDestination()
    {
        // If Enemy Destination hasn't been Set
        if (currState.GetDelta("EnemyDistanceToDestination") <= 0)
        {
            currState.UpdateDelta("EnemyDestinationPosX", currState.GetDelta("EnemyPosX"));
            currState.UpdateDelta("EnemyDestinationPosY", currState.GetDelta("EnemyPosY"));
        }

        // Calculate Player Distance To Destination
        float x = currState.GetDelta("EnemyDestinationPosX") - currState.GetDelta("PlayerPosX");
        float y = currState.GetDelta("EnemyDestinationPosY") - currState.GetDelta("PlayerPosY");
        float playerDistanceToEnemyDestination = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

        if (currState.GetDelta("EnemyDistanceToPlayer") > 20.0f &&
            playerDistanceToEnemyDestination > 25.0f)
        {
            // Set new destination
            // Get random number in range
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(5, 15);

            float playerX = currState.GetDelta("PlayerPosX");
            float playerY = currState.GetDelta("PlayerPosY");
            float destinationX = dist(gen);
            float destinationY = dist(gen);
            if (playerX > 0)
            {
                if (playerY > 0)
                {
                    destinationX = playerX - destinationX;
                    destinationY = playerY - destinationY;
                }
                else
                {
                    destinationX = playerX - destinationX;
                    destinationY = playerY + destinationY;
                }
            }
            else
            {
                if (playerY > 0)
                {
                    destinationX = playerX + destinationX;
                    destinationY = playerY - destinationY;
                }
                else
                {
                    destinationX = playerX + destinationX;
                    destinationY = playerY + destinationY;
                }
            }

            // Update deltas
            currState.UpdateDelta("EnemyDestinationPosX", destinationX);
            currState.UpdateDelta("EnemyDestinationPosY", destinationY);
        }
        else if (
            currState.GetDelta("EnemyDistanceToDestination") < 15.0f &&
            (currState.GetDelta("EnemyDestinationPosX") != currState.GetDelta("PlayerPosX") ||
            currState.GetDelta("EnemyDestinationPosY") != currState.GetDelta("PlayerPosY")))
        {
            // Change destination to player
            float x = currState.GetDelta("PlayerPosX");
            float y = currState.GetDelta("PlayerPosY");
            currState.UpdateDelta("EnemyDestinationPosX", x);
            currState.UpdateDelta("EnemyDestinationPosY", y);
        }

        // Update distance
        x = currState.GetDelta("EnemyPosX") - currState.GetDelta("EnemyDestinationPosX");
        y = currState.GetDelta("EnemyPosY") - currState.GetDelta("EnemyDestinationPosY");
        float enemyDistanceToDestination = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
        currState.UpdateDelta("EnemyDistanceToDestination", enemyDistanceToDestination);
    }

    /* 
      Reward calculation of the agent acting in the environment.
      returns:
            - The calculated reward.
    */
    float CalculateReward()
    {
        // Initialize rewards
        float reward = 0.0f;

        // Get previous state (environment hasn't been updated yet in the training loop)
        GameAgent::State prevState = env->GetCurrState();

        // AI takes Damage
        float changeInEnemyHp = prevState.GetDelta("EnemyHp") - currState.GetDelta("EnemyHp");
        reward -= changeInEnemyHp;

        // Player takes Damage
        float changeInPlayerHp = prevState.GetDelta("PlayerHp") - currState.GetDelta("PlayerHp");
        reward += changeInPlayerHp;
       
        // Calculate Enemy Weapon 1's distance from player
        float x = currState.GetDelta("EnemyWeapon1PosX") - currState.GetDelta("PlayerPosX");
        float y = currState.GetDelta("EnemyWeapon1PosY") - currState.GetDelta("PlayerPosY");
        float enemyWeapon1Distance = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

        // Calculate Enemy Weapon 2's distance from player
        x = currState.GetDelta("EnemyWeapon2PosX") - currState.GetDelta("PlayerPosX");
        y = currState.GetDelta("EnemyWeapon2PosY") - currState.GetDelta("PlayerPosY");
        float enemyWeapon2Distance = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

        // AI Attacks the Player (Mostly for shield)
        if ((currState.GetDelta("EnemyWeapon1Active") > 0 && enemyWeapon1Distance < 10.0f) ||
            (currState.GetDelta("EnemyWeapon2Active") > 0 && enemyWeapon2Distance < 10.0f))
            reward += 2.0f;

        // Player Wins Game
        if (currState.GetDelta("EnemyHp") <= 0) 
            reward -= 50.0f;

        // AI Wins Game
        if (currState.GetDelta("PlayerHp") <= 0)
            reward += 50.0f;

        // Enemy's Distance To Destination
        float changeInDistance = prevState.GetDelta("EnemyDistanceToDestination") - currState.GetDelta("EnemyDistanceToDestination");
        reward += changeInDistance;

        // Attacked in direction of Player
        if (currState.GetDelta("EnemyWeapon1") != 1 &&
            (predictedAction[0] == 4.0f || predictedAction[0] == 5.0f))
        {
            // Above player
            if (currState.GetDelta("EnemyPosY") > currState.GetDelta("PlayerPosY") &&
                (currState.GetDelta("EnemyPosX") - currState.GetDelta("PlayerPosX")) < 7.0f &&
                (currState.GetDelta("EnemyPosX") - currState.GetDelta("PlayerPosX")) > -7.0f &&
                currState.GetDelta("EnemyDirection") == 2)
                reward += 1.0f;

            // Below player
            if (currState.GetDelta("EnemyPosY") < currState.GetDelta("PlayerPosY") &&
                (currState.GetDelta("EnemyPosX") - currState.GetDelta("PlayerPosX")) < 7.0f &&
                (currState.GetDelta("EnemyPosX") - currState.GetDelta("PlayerPosX")) > -7.0f &&
                currState.GetDelta("EnemyDirection") == 1)
                reward += 1.0f;

            // Right of player
            if (currState.GetDelta("EnemyPosX") > currState.GetDelta("PlayerPosX") &&
                (currState.GetDelta("EnemyPosY") - currState.GetDelta("PlayerPosY")) < 7.0f &&
                (currState.GetDelta("EnemyPosY") - currState.GetDelta("PlayerPosY")) > -7.0f &&
                currState.GetDelta("EnemyDirection") == 4)
                reward += 1.0f;

            // Left of player
            if (currState.GetDelta("EnemyPosX") > currState.GetDelta("PlayerPosX") &&
                (currState.GetDelta("EnemyPosY") - currState.GetDelta("PlayerPosY")) < 7.0f &&
                (currState.GetDelta("EnemyPosY") - currState.GetDelta("PlayerPosY")) > -7.0f &&
                currState.GetDelta("EnemyDirection") == 3)
                reward += 1.0f;
        }

        // Penalty for not doing anything 
        if (reward == 0)
            reward -= 0.1f;

        return reward;
    }

    /*
      Creates line plots for each of the different enemy weapon combinations and their earned rewards.
    */
    void VisualizeWeaponRewards()
    {
        GameAgent::Visualizer visualizer;
        for (auto weaponRewardHistory : weaponsRewardHistory)
        {
            std::vector<float> Y = weaponRewardHistory.second;
            std::vector<float> X(Y.size());
            std::iota(X.begin(), X.end(), 0);
            std::string filename = "Weapon" + weaponRewardHistory.first + "RewardHistory.png";
            std::string labelname = "Weapon " + weaponRewardHistory.first.substr(0, 1) +
                                    " and " + weaponRewardHistory.first.substr(1, 1) +
                                    " Rewards";

            visualizer.PlotLine(X, Y, filename, labelname);
        }
    }

    /* 
      Creates bar graphs for each player weapon combination and their average utilities. 
      Useful for comparing the usefulness of each enemy weapon combination.
    */
    void VisualizeWeaponUtilities()
    {
        GameAgent::Visualizer visualizer; 
        for (auto weaponComp : TrainingData::GetWeaponComps())
        {
            // Create filename
            std::string filename = "Weapon" + std::to_string(weaponComp[0]) + 
                                   "and" + std::to_string(weaponComp[1]) + "Utilities.png";

            // Build search state
            GameAgent::State searchState;
            searchState.AddDelta("PlayerWeapon1", weaponComp[0]);
            searchState.AddDelta("PlayerWeapon2", weaponComp[1]);
            searchState.AddDelta("EnemyWeapon1");
            searchState.AddDelta("EnemyWeapon2");

            // Build data
            std::vector<std::string> xlabels;
            std::vector<float> ydata;
            for (auto weapons : TrainingData::GetWeaponComps())
            {
                // Search utility storage for weapon combination
                searchState.UpdateDelta("EnemyWeapon1", weapons[0]);
                searchState.UpdateDelta("EnemyWeapon2", weapons[1]);
                auto utilitiesStates = agent->SearchUtilityStorage(searchState);

                // If empty search for the next one
                if (utilitiesStates.empty())
                    continue;

                // Push back data
                ydata.push_back(agent->SearchUtilityStorage(searchState)[0].second);

                // Create label for that data
                std::string xlabel = std::to_string(weapons[0]) + " and " + std::to_string(weapons[1]);
                xlabels.push_back(xlabel);
            }

            // If no data found
            if (ydata.empty())
                continue;

            // Graph
            visualizer.PlotBar(xlabels, ydata, filename);
        }
    }
    
    /* The agent of the game environment. */
    std::shared_ptr<GameAgent::Agent> agent;
    
    /* The agent's environment. */
    std::shared_ptr<GameAgent::Environment> env;

    /* The current state of the environment. */
    GameAgent::State currState;

    /* Reference to the enemy script. */
    ComponentHandle<Script> enemyScript;

    /* Predicted action of the agent. */
    std::vector<float> predictedAction;

    /* Reference to the player script. */
    ComponentHandle<Script> playerScript;

    /* Current moveset of the player (used for automatic training). */
    std::vector<unsigned int> currPlayerMoveSet;

    /* The current index of the current player moveset (used for automatic training). */
    int currMoveSetIndex;

    /* The best average reward earned by the agent. */
    float bestAvgReward = -std::numeric_limits<float>::max();

    /* Stores the reward history of all played episodes. */
    std::vector<float> episodesRewardHistory;

    /* Stores the reward history of each weapon combination (used for visualization).*/
    std::unordered_map<std::string, std::vector<float>> weaponsRewardHistory;

    /* The current weapons being used. Is a key for the weapons reward history. */
    std::string currWeaponKey;

    /* The win count of the agent. */
    unsigned int winCount = 0;

    /* The number of total episodes. */
    unsigned int nEpisodes = 0;

    /* The max length of an episode. */
    unsigned int maxEpisodeLength = 0;

    /* The current episode. */
    unsigned int nEpisodeCount = 1;

    /* Determines whether the arena scene is loaded or not. */
    bool arenaLoaded = false;

    /* TODO: Band-aid solution to Game being paused after a game and the agent still needs to save. */
    bool gameEndedAndAgentSaved = false;

    /* The current player weapon set index which is used for switching player weapons after n episodes during automatic training. */
    int currPlayerWeaponSetIndex = 0;

};
