#include "pausemenuScript.h"

#include "../sceneManager.h"
#include "../components.h"
#include "../logger.h"
#include "../input.h"
#include "../ECS.h"

PauseMenuScript::PauseMenuScript(entityx::Entity* entity) : CScript(entity) {}

void PauseMenuScript::start() {

    // TODO: Currently has a O(N) runtime looking for two objects, can probably reduce
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "ResumeText")
            resumeText = entity;

        if (entityName.get()->name == "EndMatchText")
            endMatchText = entity;

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
    hideEntities(isActive);

    currOption = 0;
    numOfOptions = 2;
    cooldownTimer = 0;
}

void PauseMenuScript::update(TimeDelta dt) {

    // Pause program
    if (Input::instance().isKeyPressed(GLFW_KEY_ESCAPE)) {
        // Display pause screen
        isActive = !isActive;
        hideEntities(isActive);

        // Pause or unpause game
        if (ECS::instance().isGamePaused())
            ECS::instance().unpauseGame();
        else
            ECS::instance().pauseGame();
    }

    // Selection made
    if (Input::instance().isKeyPressed(GLFW_KEY_ENTER)) {
        if (currOption == 0) {
            // Display pause screen
            isActive = !isActive;
            hideEntities(isActive);

            // Unpause game
            ECS::instance().unpauseGame();
        }

        if (currOption == 1) {
            SceneManager::instance().loadScene("LoadoutSelection");
        }
    }

    if (!isActive)
        return;

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
    }

}

void PauseMenuScript::hideEntities(bool active) {
    ComponentHandle<Active> activeComp = pauseText.component<Active>();
    activeComp.get()->isActive = active;

    activeComp = resumeText.component<Active>();
    activeComp.get()->isActive = active;

    activeComp = endMatchText.component<Active>();
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
