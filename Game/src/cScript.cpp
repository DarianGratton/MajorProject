#include "CScript.h"

#include "Logger.h"
#include "ECS.h"

CScript::CScript(Entity* entity) 
{
    this->entity = ECS::Instance().entities.get(entity->id());
}

void CScript::BeginContact(Entity* entityA, Entity* entityB) 
{
    LOG_WARN("CScript: Abstract class' start contact called.");
}

void CScript::EndContact(Entity* entityA, Entity* entityB) 
{
    LOG_WARN("CScript: Abstract class' end contact called."); 
}
