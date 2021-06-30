#include "playerScript.h"

#include "../logger.h"

PlayerScript::PlayerScript(entityx::Entity* entity) : CScript(entity) {

}

void PlayerScript::start() {

}

void PlayerScript::update() {
    LOG_TRACE("Player script update");
}