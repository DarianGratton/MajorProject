#pragma once

#include <vector>

#include "Scene.h"

class SceneManager {
public:
    static SceneManager& instance() {
        static SceneManager *instance = new SceneManager();
        return *instance;
    }

    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;

    void addScene(std::string scenename, std::string filename);
    void loadScene(std::string scenename);

private:
    SceneManager() {}
    bool isSceneLoaded = false;

    std::vector<Scene*> scenes;
};