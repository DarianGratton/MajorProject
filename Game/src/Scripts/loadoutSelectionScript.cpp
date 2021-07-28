#include "loadoutSelectionScript.h"

#include "../sceneManager.h"
#include "../components.h"
#include "../logger.h"
#include "../input.h"
#include "../ECS.h"

LoadoutSelectionScript::LoadoutSelectionScript(entityx::Entity* entity) : CScript(entity) {
    arrowPosOffset = 70.0f;
}

void LoadoutSelectionScript::start() {

    // TODO: Currently has a O(N) runtime looking for two objects, can probably reduce
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::instance().entities.entities_with_components(entityName)) {
        
        entityName = e.component<Name>();
        if (entityName.get()->name == "BackText")
            backOption = e;

        if (entityName.get()->name == "FightText")
            fightOption = e;

        if (entityName.get()->name == "WeaponOption1")
            weaponOption1 = e;

        if (entityName.get()->name == "WeaponOption2")
            weaponOption2 = e;

        if (entityName.get()->name == "WeaponOption3")
            weaponOption3 = e;

        if (entityName.get()->name == "WeaponOption4")
            weaponOption4 = e;

        if (entityName.get()->name == "WeaponOption5")
            weaponOption5 = e;

        if (entityName.get()->name == "WeaponPreview")
            weaponPreview = e;
    }

    currOption = 1;
    numOfOptions = 7;
    cooldownBetweenKeys = 0;
}

void LoadoutSelectionScript::update(TimeDelta dt) {

    // Selection made
    if (Input::instance().isKeyPressed(GLFW_KEY_ENTER)) {
        if (currOption == 0) {
            SceneManager::instance().loadScene("MainMenu");
        }

        if (currOption == 6) {
            SceneManager::instance().loadScene("Arena");
        }
    }

    if (cooldownBetweenKeys != 0) {
        cooldownBetweenKeys--;
        return;
    }

    // Browse options
    if (Input::instance().isKeyPressed(GLFW_KEY_W)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        
        // Back Text -> Option 4
        if (currOption == 0) {
            transform.get()->xpos = weaponOption4.component<Transform>().get()->xpos - arrowPosOffset;
            transform.get()->ypos = weaponOption4.component<Transform>().get()->ypos;
            currOption = 4;
        }

        // Option 4 -> Option 1
        else if (currOption == 4) {
            transform.get()->xpos = weaponOption1.component<Transform>().get()->xpos - arrowPosOffset;
            transform.get()->ypos = weaponOption1.component<Transform>().get()->ypos;
            currOption = 1;
        }

        // Option 5 -> Option 2
        else if (currOption == 5) {
            transform.get()->xpos = weaponOption2.component<Transform>().get()->xpos - arrowPosOffset;
            transform.get()->ypos = weaponOption2.component<Transform>().get()->ypos;
            currOption = 2;
        }

        // Fight Text -> Option 5
        else if (currOption == 6) {
            transform.get()->xpos = weaponOption5.component<Transform>().get()->xpos - arrowPosOffset;
            transform.get()->ypos = weaponOption5.component<Transform>().get()->ypos;
            currOption = 5;
        }

        cooldownBetweenKeys = 3;
    }

    if (Input::instance().isKeyPressed(GLFW_KEY_S)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        
        // Option 1 -> Option 4
        if (currOption == 1) {
            transform.get()->xpos = weaponOption4.component<Transform>().get()->xpos - arrowPosOffset;
            transform.get()->ypos = weaponOption4.component<Transform>().get()->ypos;
            currOption = 4;
        }

        // Option 4 -> Back Text
        else if (currOption == 4) {
            transform.get()->xpos = backOption.component<Transform>().get()->xpos - 35.0f;
            transform.get()->ypos = backOption.component<Transform>().get()->ypos + 12.5f;
            currOption = 0;
        }

        // Option 2 -> Option 5
        else if (currOption == 2) {
            transform.get()->xpos = weaponOption5.component<Transform>().get()->xpos - arrowPosOffset;
            transform.get()->ypos = weaponOption5.component<Transform>().get()->ypos;
            currOption = 5;
        }

        // Option 3 -> Option 5
        else if (currOption == 3) {
            transform.get()->xpos = weaponOption5.component<Transform>().get()->xpos - arrowPosOffset;
            transform.get()->ypos = weaponOption5.component<Transform>().get()->ypos;
            currOption = 5;
        }

        // Option 5 -> Fight Text
        else if (currOption == 5) {
            transform.get()->xpos = fightOption.component<Transform>().get()->xpos - 35.0f;
            transform.get()->ypos = fightOption.component<Transform>().get()->ypos + 12.5f;
            currOption = 6;
        }

        cooldownBetweenKeys = 3;
    }

    if (Input::instance().isKeyPressed(GLFW_KEY_D)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();

        // Option 1 -> Option 2
        if (currOption == 1) {
            transform.get()->xpos = weaponOption2.component<Transform>().get()->xpos - arrowPosOffset;
            currOption = 2;
        }

        // Option 2 -> Option 3
        else if (currOption == 2) {
            transform.get()->xpos = weaponOption3.component<Transform>().get()->xpos - arrowPosOffset;
            currOption = 3;
        }

        // Option 4 -> Option 5
        else if (currOption == 4) {
            transform.get()->xpos = weaponOption5.component<Transform>().get()->xpos - arrowPosOffset;
            currOption = 5;
        }

        cooldownBetweenKeys = 3;
    }

    if (Input::instance().isKeyPressed(GLFW_KEY_A)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();

        // Option 3 -> Option 2
        if (currOption == 3) {
            transform.get()->xpos = weaponOption2.component<Transform>().get()->xpos - arrowPosOffset;
            currOption = 2;
        }

        // Option 2 -> Option 1
        else if (currOption == 2) {
            transform.get()->xpos = weaponOption1.component<Transform>().get()->xpos - arrowPosOffset;
            currOption = 1;
        }

        // Option 5 -> Option 4
        else if (currOption == 5) {
            transform.get()->xpos = weaponOption4.component<Transform>().get()->xpos - arrowPosOffset;
            currOption = 4;
        }

        cooldownBetweenKeys = 3;
    }

    // Update weapon preview
    ComponentHandle<TextureComp> textureComp = weaponPreview.component<TextureComp>();
    switch(currOption) {
        case 1:
            textureComp.get()->setTexture("src/Assets/textures/Sword.png");
            break;
        case 2:
            textureComp.get()->setTexture("src/Assets/textures/Shield.png");
            break;
        case 3:
            textureComp.get()->setTexture("src/Assets/textures/ArrowUp.png");
            break;
        case 4:
            textureComp.get()->setTexture("src/Assets/textures/Grenade.png");
            break;
        case 5:
            textureComp.get()->setTexture("src/Assets/textures/Gauntlet.png");
            break;
        default:
            break;
    }

}