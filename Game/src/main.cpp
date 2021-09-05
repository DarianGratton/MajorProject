#include "sceneManager.h"
#include "Game.h"

int main(int argc, char** argv) 
{   
    // Init Game
    if (!Game::Instance().Init()) 
        return -1;
    
    // Setup Scenes 
    SceneManager::instance().addScene("MainMenu", "src/Assets/scenes/mainmenu.xml");
    SceneManager::instance().addScene("LoadoutSelection", "src/Assets/scenes/loadoutselection.xml");
    SceneManager::instance().addScene("Arena", "src/Assets/scenes/arena.xml");
    SceneManager::instance().loadScene("MainMenu");

    // Run Game Loop
    Game::Instance().Run();
    Game::Instance().CleanUp();

    return 0;
}
