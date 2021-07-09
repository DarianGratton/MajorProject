#include "playerScript.h"

#include "../logger.h"

PlayerScript::PlayerScript(entityx::Entity* entity) : CScript(entity) {

}

void PlayerScript::start() {
    LOG_TRACE("Player script start");
}

void PlayerScript::update() {
    LOG_TRACE("Player script update");
}