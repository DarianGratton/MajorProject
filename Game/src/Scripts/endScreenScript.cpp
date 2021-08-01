#include "endScreenScript.h"

#include "../sceneManager.h"
#include "../components.h"
#include "../scripts.h"
#include "../logger.h"
#include "../input.h"
#include "../ECS.h"

EndScreenScript::EndScreenScript(entityx::Entity* entity) : CScript(entity) {}

void EndScreenScript::start() {

    // TODO: Currently has a O(N) runtime looking for two objects, can probably reduce
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::instance().entities.entities_with_components(entityName)) {
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
    hideEntities(isActive);

    currOption = 0;
    numOfOptions = 2;
    cooldownTimer = 0;
}

void EndScreenScript::update(TimeDelta dt) {

    if (!isActive) {
        if (player.valid()) {
            ComponentHandle<Script> playerScript = player.component<Script>();
            int currHp = reinterpret_cast<PlayerScript*>(playerScript.get()->script)->getHealth();
            if (currHp <= 0) {
                // Display end screen
                isActive = !isActive;
                hideEntities(isActive);

                // Pause game
                ECS::instance().pauseGame();

                // Change text to proper text
                ComponentHandle<TextSprite> endText = endingText.component<TextSprite>();
                endText.get()->text = "You Lose!";
            }
        }

        if (enemy.valid()) {
            ComponentHandle<Script> enemyScript = enemy.component<Script>();
            int currHp = reinterpret_cast<EnemyScript*>(enemyScript.get()->script)->getHealth();
            if (currHp <= 0) {
                // Display end screen
                isActive = !isActive;
                hideEntities(isActive);

                // Pause game
                ECS::instance().pauseGame();

                // Change text to proper text
                ComponentHandle<TextSprite> endText = endingText.component<TextSprite>();
                endText.get()->text = "You Win!";
            }
        }
    } else {
        // Update cooldown
        float cooldown = cooldownTimer - dt;
        if (cooldown <= 0) {
            cooldownTimer = 0;
        } else {
            cooldownTimer = cooldown;
            return;
        }

        // Selection made
        if (Input::instance().isKeyPressed(GLFW_KEY_ENTER)) {
            // Restart
            if (currOption == 0) {
                SceneManager::instance().loadScene("Arena");
            }

            // Exit
            if (currOption == 1) {
                SceneManager::instance().loadScene("LoadoutSelection");
            }
        }

        // Browse options
        if (Input::instance().isKeyPressed(GLFW_KEY_W)) {
            ComponentHandle<Transform> transform = entity.component<Transform>();
            if (currOption == 0) {
                transform.get()->ypos -= (5.0f * (numOfOptions - 1));
                currOption = numOfOptions - 1;
            } else {
                transform.get()->ypos += 5.0f;
                currOption--;
            }

            cooldownTimer = 0.2f;
        }

        if (Input::instance().isKeyPressed(GLFW_KEY_S)) {
            ComponentHandle<Transform> transform = entity.component<Transform>();
            if (currOption == (numOfOptions - 1)) {
                transform.get()->ypos += (5.0f * (numOfOptions - 1));
                currOption = 0;
            } else {
                transform.get()->ypos -= 5.0f;
                currOption++;
            }

            cooldownTimer = 0.2f;
        }
    }

}

void EndScreenScript::hideEntities(bool active) {
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
