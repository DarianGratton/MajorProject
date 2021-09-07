#include "MainmenuScript.h"

#include "../SceneManager.h"
#include "../Components.h"
#include "../Logger.h"
#include "../Input.h"
#include "../ECS.h"

MainMenuScript::MainMenuScript(entityx::Entity* entity) : CScript(entity) {}

void MainMenuScript::Start() 
{
    // TODO: Currently has a O(N) runtime looking for two objects, can probably reduce
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "StartGameText")
            startText = entity;

        if (entityName.get()->name == "QuitGameText")
            quitText = entity;
    }

    currOption = 0;
    numOfOptions = 2;
    cooldownBetweenKeys = 0;
    sceneLoadDelay = 0.2;
}

void MainMenuScript::Update(TimeDelta dt) 
{
    // Update delay
    float delay = sceneLoadDelay - dt;
    if (delay <= 0) 
    {
        sceneLoadDelay = 0;
    } 
    else 
    {
        sceneLoadDelay = delay;
        return;
    }

    // Selection made
    if (Input::Instance().IsKeyPressed(GLFW_KEY_ENTER)) 
    {
        if (currOption == 0) 
            SceneManager::Instance().LoadScene("LoadoutSelection");

        if (currOption == 1) 
            ECS::Instance().EndGame();
    }

    // Exit program
    if (Input::Instance().IsKeyPressed(GLFW_KEY_ESCAPE))
        ECS::Instance().EndGame();

    if (cooldownBetweenKeys != 0) 
    {
        cooldownBetweenKeys--;
        return;
    }

    // Browse options
    if (Input::Instance().IsKeyPressed(GLFW_KEY_W)) 
    {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        if (currOption == 0) 
        {
            transform.get()->ypos -= (50.0f * (numOfOptions - 1));
            currOption = numOfOptions - 1;
        } 
        else 
        {
            transform.get()->ypos += 50.0f;
            currOption--;
        }
        cooldownBetweenKeys = 3;
    }

    if (Input::Instance().IsKeyPressed(GLFW_KEY_S)) 
    {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        if (currOption == (numOfOptions - 1)) 
        {
            transform.get()->ypos += (50.0f * (numOfOptions - 1));
            currOption = 0;
        } 
        else 
        {
            transform.get()->ypos -= 50.0f;
            currOption++;
        }
        cooldownBetweenKeys = 3;
    }
}