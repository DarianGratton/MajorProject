#include "Game.h"
#include "SceneManager.h"

#include <iostream>

#include <GameAgent/GameAgent.h>

int main(int argc, char** argv) 
{   
    // GameAgent Test
    /*GameAgent::Networks::ACERParameters params;

    GameAgent::State state;
    state.AddDelta("X", 90);
    state.AddDelta("Y", -90);

    std::shared_ptr<GameAgent::Environment> env = std::make_shared<GameAgent::Environment>(state);
    GameAgent::Agent agent(env, params);

    GameAgent::Visualizer visualizer;
    std::vector<float> X = { 1, 2, 3, 4 };
    std::vector<float> Y = { 1, 2, 3, 4 };
    visualizer.PlotLine(X, Y, "LinePlotTest.png", "Stuff");

    exit(0);*/

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
