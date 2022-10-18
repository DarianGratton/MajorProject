#pragma once

#include <vector>

#include "Scene.h"

class SceneManager 
{
public:
    static SceneManager& Instance() 
    {
        static SceneManager *instance = new SceneManager();
        return *instance;
    }

    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;

    void AddScene(std::string scenename, std::string filename);
    bool LoadScene(std::string scenename);

private:
    SceneManager() {}
    bool isSceneLoaded = false;

    std::vector<Scene*> scenes;
};