#pragma once

#include <entityx/entityx.h>

#include "../Components.h"
#include "../Logger.h"

using namespace entityx;

// TODO: Move temp
class AudioSystem : public System<AudioSystem> 
{
public:
    void update(EntityManager& es, EventManager& events, TimeDelta dt) override 
    {
        es.each<Audio>([this, dt](Entity entity, Audio &audioComp) 
        {
            if (audioComp.playOnStart && !temp) 
            {
                temp = true;
                audioComp.sound->Play();
            }
            audioComp.sound->SetVolume(audioComp.volume);
        });
    }

private:
    bool temp;
};
