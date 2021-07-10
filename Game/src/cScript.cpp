#include "cScript.h"

#include "ECS.h"

CScript::CScript(Entity* entity) {
    this->entity = ECS::instance().entities.get(entity->id());
}
