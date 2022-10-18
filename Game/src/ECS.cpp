#include "ECS.h"

#include "Systems.h"

void ECS::Init() 
{
    systems.add<AudioSystem>();
    systems.add<ScriptSystem>();
    systems.add<PhysicsSystem>();
    systems.add<RenderingSystem>();
    systems.add<LearningAgentSystem>();
    systems.configure();
}

void ECS::Update(TimeDelta dt) 
{
    systems.update<AudioSystem>(dt);
    systems.update<ScriptSystem>(dt);
    systems.update<PhysicsSystem>(dt);
    systems.update<LearningAgentSystem>(dt);
    systems.update<RenderingSystem>(dt);
}
