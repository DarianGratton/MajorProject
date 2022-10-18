#pragma once

#include <gtest/gtest.h>

#include "../src/Scene.h"
#include "../src/SceneManager.h"
#include "../src/Game.h"

TEST (LoadScene, ValidScene)
{
    // Init Game
    Game::Instance().Init();
    
    // Run Test
    SceneManager::Instance().AddScene("MainMenu", "src/Assets/scenes/mainmenu.xml");
    bool sceneLoaded = SceneManager::Instance().LoadScene("MainMenu");
    EXPECT_TRUE(sceneLoaded);
}

TEST (LoadScene, InvalidSceneName)
{
    // Run Test
    bool sceneLoaded = SceneManager::Instance().LoadScene("MainMeni");
    EXPECT_FALSE(sceneLoaded);
}

TEST (LoadScene, ValidSceneNameWithInvalidFilepath)
{
    // Run Test
    SceneManager::Instance().AddScene("MainSdf", "src/Assets/scenes/mainmenuq.xml");
    bool sceneLoaded = SceneManager::Instance().LoadScene("MainSdf");
    EXPECT_FALSE(sceneLoaded);
}
