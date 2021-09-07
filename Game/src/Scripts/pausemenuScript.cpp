#include "PausemenuScript.h"

#include "../SceneManager.h"
#include "../Components.h"
#include "../Logger.h"
#include "../Input.h"
#include "../ECS.h"

PauseMenuScript::PauseMenuScript(entityx::Entity* entity) : CScript(entity) {}

void PauseMenuScript::Start() 
{
    // TODO: Currently has a O(N) runtime looking for two objects, can probably reduce
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "ResumeText")
            resumeText = entity;

        if (entityName.get()->name == "ExitText")
            exitText = entity;

        if (entityName.get()->name == "PauseBackground")
            pauseBackground = entity;

        if (entityName.get()->name == "PauseText")
            pauseText = entity;

        if (entityName.get()->name == "PlayerHpText")
            playerHpText = entity;

        if (entityName.get()->name == "EnemyHpText")
            enemyHpText = entity;
    }

    isActive = false;
    HideEntities(isActive);

    currOption = 0;
    numOfOptions = 2;
    cooldownTimer = 0;
}

void PauseMenuScript::Update(TimeDelta dt) 
{
    // Update cooldown
    float cooldown = cooldownTimer - dt;
    if (cooldown <= 0) 
    {
        cooldownTimer = 0;
    } 
    else 
    {
        cooldownTimer = cooldown;
        return;
    }

    // Pause program
    if (Input::Instance().IsKeyPressed(GLFW_KEY_ESCAPE)) 
    {
        // Display pause screen
        isActive = !isActive;
        HideEntities(isActive);

        // Pause or unpause game
        if (ECS::Instance().IsGamePaused())
            ECS::Instance().UnpauseGame();
        else
            ECS::Instance().PauseGame();

        cooldownTimer = 0.2f;
    }

    if (!isActive)
        return;

    // Selection made
    if (Input::Instance().IsKeyPressed(GLFW_KEY_ENTER)) 
    {
        if (currOption == 0) 
        {
            // Display pause screen
            isActive = !isActive;
            HideEntities(isActive);

            // Unpause game
            ECS::Instance().UnpauseGame();
        }

        if (currOption == 1) 
            SceneManager::Instance().LoadScene("LoadoutSelection");

        cooldownTimer = 0.2f;
    }

    // Browse options
    if (Input::Instance().IsKeyPressed(GLFW_KEY_W)) 
    {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        if (currOption == 0) 
        {
            transform.get()->ypos -= (5.0f * (numOfOptions - 1));
            currOption = numOfOptions - 1;
        } 
        else 
        {
            transform.get()->ypos += 5.0f;
            currOption--;
        }

        cooldownTimer = 0.2f;
    }

    if (Input::Instance().IsKeyPressed(GLFW_KEY_S)) 
    {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        if (currOption == (numOfOptions - 1)) 
        {
            transform.get()->ypos += (5.0f * (numOfOptions - 1));
            currOption = 0;
        } 
        else 
        {
            transform.get()->ypos -= 5.0f;
            currOption++;
        }

        cooldownTimer = 0.2f;
    }
}

void PauseMenuScript::HideEntities(bool active) 
{
    ComponentHandle<Active> activeComp = pauseText.component<Active>();
    activeComp.get()->isActive = active;

    activeComp = resumeText.component<Active>();
    activeComp.get()->isActive = active;

    activeComp = exitText.component<Active>();
    activeComp.get()->isActive = active;

    activeComp = pauseBackground.component<Active>();
    activeComp.get()->isActive = active;

    activeComp = entity.component<Active>();
    activeComp.get()->isActive = active;

    activeComp = playerHpText.component<Active>();
    activeComp.get()->isActive = !active;

    activeComp = enemyHpText.component<Active>();
    activeComp.get()->isActive = !active;
}
