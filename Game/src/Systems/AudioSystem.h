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
            if (audioComp.playOnStart && !audioComp.isPlaying) 
            {
                audioComp.sound->Play();
                audioComp.isPlaying = true;
            }
            audioComp.sound->SetVolume(audioComp.volume);
        });
    }
};
