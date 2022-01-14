#include "SceneManager.h"

#include <vector>

#include "Game.h"
#include "ECS.h"
#include "Events.h"
#include "Components.h"
#include "Logger.h"

void SceneManager::AddScene(std::string scenename, std::string filename) 
{
    scenes.push_back(new Scene(scenename, filename));
}

bool SceneManager::LoadScene(std::string sceneName) 
{
    Scene* scene;
    bool sceneFound = false;
    for (int i = 0; i < scenes.size(); ++i) 
    {
        scene = scenes.at(i);
        if (scene->GetName() == sceneName) 
        {
            sceneFound = true;
            break;
        }
    }

    if (sceneFound) {
        // Events for unloading scene
        if (isSceneLoaded) 
        {
            // Reset all entities
            ECS::Instance().entities.reset();
        }

        // Load scene
        if (!scene->Load())
        {
            return 0;
        }
        ECS::Instance().events.emit<SceneLoad>(scene->GetName(), &ECS::Instance().entities);
        Game::Instance().UnpauseGame();
        isSceneLoaded = true;

        return 1;
    } 
    else 
    {
        LOG_ERROR("SceneManager::loadScene failed: Scene " + sceneName + " not found");
        return 0;
    }
}
