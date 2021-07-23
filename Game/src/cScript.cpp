#include "cScript.h"

#include "logger.h"
#include "ECS.h"

CScript::CScript(Entity* entity) {
    this->entity = ECS::instance().entities.get(entity->id());
}

void CScript::beginContact() {
    LOG_WARN("CScript: Abstract class' start contact called.");
}

void CScript::endContact() {
    LOG_WARN("CScript: Abstract class' end contact called."); 
}
