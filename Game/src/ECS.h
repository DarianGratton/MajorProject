#pragma once

#include <entityx/entityx.h>

using namespace entityx;

class ECS : public EntityX 
{
public:
    static ECS& Instance() 
    {
        static ECS *instance = new ECS();
        return *instance;
    }

    ECS(ECS const&) = delete;
    void operator=(ECS const&) = delete;

    void Init();
    void Update(TimeDelta dt);
    
    // TODO: Shouldn't be in ECS, should be in a game class or something that handles the game loop and stuff.
    // This is a quick solution to fix a low priority problem.
    void EndGame(); 
    bool gameEnded;

    void PauseGame();
    void UnpauseGame();
    bool IsGamePaused();
    bool gamePaused;

private:
    ECS() {}
};