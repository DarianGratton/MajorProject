#include "Game.h"
#include "SceneManager.h"

#include <iostream>

#include <GameAgent/GameAgent.h>

int main(int argc, char** argv) 
{   
    // Init Game
    if (!Game::Instance().Init()) 
        return -1;
    
    // Setup Scenes 
    SceneManager::Instance().AddScene("MainMenu", "src/Assets/scenes/mainmenu.xml");
    SceneManager::Instance().AddScene("LoadoutSelection", "src/Assets/scenes/loadoutselection.xml");
    SceneManager::Instance().AddScene("Arena", "src/Assets/scenes/arena.xml");
    SceneManager::Instance().LoadScene("MainMenu");

    // Run Game Loop
    Game::Instance().Run();
    Game::Instance().CleanUp();

    return 0;
}
