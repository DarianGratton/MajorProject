#include "sceneManager.h"

#include <vector>

#include "ECS.h"
#include "events.h"
#include "components.h"
#include "logger.h"

void SceneManager::addScene(std::string scenename, std::string filename) {
    scenes.push_back(new Scene(scenename, filename));
}

bool SceneManager::loadScene(std::string sceneName) {
    Scene* scene;
    bool sceneFound = false;
    for (int i = 0; i < scenes.size(); ++i) {
        scene = scenes.at(i);
        if (scene->getName() == sceneName) {
            sceneFound = true;
            break;
        }
    }

    if (sceneFound) {
        // Events for unloading scene
        if (isSceneLoaded) {
            // Reset all entities
            ECS::instance().entities.reset();
        }

        // Load scene
        scene->load();
        ECS::instance().events.emit<SceneLoad>(scene->getName(), &ECS::instance().entities);
        ECS::instance().unpauseGame();
        isSceneLoaded = true;

        return 1;
    } else {
        LOG_ERROR("SceneManager::loadScene failed: Scene " + sceneName + " not found");
        return 0;
    }
}
