#include "EndScreenScript.h"

#include "../SceneManager.h"
#include "../Components.h"
#include "../Scripts.h"
#include "../Logger.h"
#include "../Input.h"
#include "../Game.h"

EndScreenScript::EndScreenScript(entityx::Entity* entity) : CScript(entity) {}

void EndScreenScript::Start() 
{
    // TODO: Currently has a O(N) runtime looking for two objects, can probably reduce
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "EndingText")
            endingText = entity;

        if (entityName.get()->name == "PauseBackground")
            pauseBackground = entity;

        if (entityName.get()->name == "RestartText")
            restartText = entity;

        if (entityName.get()->name == "ExitText")
            exitText = entity;

        if (entityName.get()->name == "PlayerHpText")
            playerHpText = entity;

        if (entityName.get()->name == "EnemyHpText")
            enemyHpText = entity;

        if (entityName.get()->name == "Player")
            player = entity;

        if (entityName.get()->name == "Enemy")
            enemy = entity;
    }

    isActive = false;
    HideEntities(isActive);

    currOption = 0;
    numOfOptions = 2;
    cooldownTimer = 0;
}

void EndScreenScript::Update(TimeDelta dt) 
{
    if (!isActive) 
    {
        if (player.valid())
        {
            ComponentHandle<Script> playerScript = player.component<Script>();
            int currHp = reinterpret_cast<PlayerScript*>(playerScript.get()->script)->GetHealth();
            if (currHp <= 0) 
            {
                Game::Instance().SetTerminalState(true);
                
                if (!Game::Instance().IsAutomaticallyTraining())
                {
                    // Display end screen
                    isActive = !isActive;
                    HideEntities(isActive);

                    // Pause game
                    Game::Instance().PauseGame();

                    // Change text to proper text
                    ComponentHandle<TextSprite> endText = endingText.component<TextSprite>();
                    endText.get()->text = "You Lose!";
                }
            }
        }

        if (enemy.valid()) 
        {
            ComponentHandle<Script> enemyScript = enemy.component<Script>();
            int currHp = reinterpret_cast<EnemyScript*>(enemyScript.get()->script)->GetHealth();
            if (currHp <= 0) 
            {
                Game::Instance().SetTerminalState(true);

                if (!Game::Instance().IsAutomaticallyTraining())
                {
                    // Display end screen
                    isActive = !isActive;
                    HideEntities(isActive);

                    // Pause game
                    Game::Instance().PauseGame();

                    // Change text to proper text
                    ComponentHandle<TextSprite> endText = endingText.component<TextSprite>();
                    endText.get()->text = "You Win!";
                }
            }
        }
    } 
    else 
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

        // Selection made
        if (Input::Instance().IsKeyPressed(GLFW_KEY_ENTER)) 
        {
            // Restart
            if (currOption == 0)
                SceneManager::Instance().LoadScene("Arena");

            // Exit
            if (currOption == 1) 
                SceneManager::Instance().LoadScene("LoadoutSelection");
        }

        // Browse options
        if (Input::Instance().IsKeyPressed(GLFW_KEY_W)) 
        {
            ComponentHandle<Transform> transform = entity.component<Transform>();
            if (currOption == 0) 
            {
                transform.get()->ypos -= (4.5f * (numOfOptions - 1));
                currOption = numOfOptions - 1;
            } 
            else 
            {
                transform.get()->ypos += 4.5f;
                currOption--;
            }

            cooldownTimer = 0.2f;
        }

        if (Input::Instance().IsKeyPressed(GLFW_KEY_S)) 
        {
            ComponentHandle<Transform> transform = entity.component<Transform>();
            if (currOption == (numOfOptions - 1)) 
            {
                transform.get()->ypos += (4.5f * (numOfOptions - 1));
                currOption = 0;
            } 
            else 
            {
                transform.get()->ypos -= 4.5f;
                currOption++;
            }

            cooldownTimer = 0.2f;
        }
    }
}

void EndScreenScript::HideEntities(bool active) 
{
    ComponentHandle<Active> activeComp = endingText.component<Active>();
    activeComp.get()->isActive = active;

    activeComp = restartText.component<Active>();
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
