#include "sceneManager.h"

void SceneManager::addScene(std::string scenename, std::string filename) {
    scenes.push_back(new Scene(scenename, filename));
}

void SceneManager::loadScene(std::string sceneName) {
    
}
