#include "mainmenuScript.h"

#include "../sceneManager.h"
#include "../components.h"
#include "../logger.h"
#include "../input.h"
#include "../ECS.h"

MainMenuScript::MainMenuScript(entityx::Entity* entity) : CScript(entity) {}

void MainMenuScript::start() {

    // TODO: Currently has a O(N) runtime looking for two objects, can probably reduce
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "StartGameText")
            startText = entity;

        if (entityName.get()->name == "QuitGameText")
            quitText = entity;
    }

    currOption = 0;
    numOfOptions = 2;
}

void MainMenuScript::update() {

    // Browse options
    if (Input::instance().isKeyPressed(GLFW_KEY_W)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        if (currOption == 0) {
            transform.get()->ypos -= (50.0f * (numOfOptions - 1));
            currOption = numOfOptions - 1;
        } else {
            transform.get()->ypos += 50.0f;
            currOption--;
        }
    }

    if (Input::instance().isKeyPressed(GLFW_KEY_S)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        if (currOption == (numOfOptions - 1)) {
            transform.get()->ypos += (50.0f * (numOfOptions - 1));
            currOption = 0;
        } else {
            transform.get()->ypos -= 50.0f;
            currOption++;
        }
    }

    // Selection made
    if (Input::instance().isKeyPressed(GLFW_KEY_ENTER)) {
        if (currOption == 0) {
            SceneManager::instance().loadScene("LoadoutSelection");
        }

        if (currOption == 1) {
            ECS::instance().endGame();
        }
    }

    // Exit program
    if (Input::instance().isKeyPressed(GLFW_KEY_ESCAPE)) {
        ECS::instance().endGame();
    }

}