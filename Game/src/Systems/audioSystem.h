#pragma once

#include <entityx/entityx.h>

#include "../components.h"
#include "../logger.h"

using namespace entityx;

class AudioSystem : public System<AudioSystem> {
public:
    void update(EntityManager& es, EventManager& events, TimeDelta dt) override {
        es.each<Audio>([this, dt](
            Entity entity, Audio &audioComp) {

            if (audioComp.playOnStart && !temp) {
                temp = true;
                audioComp.sound->play();
            }
            audioComp.sound->setVolume(audioComp.volume);

        });
    }
private:
    bool temp;
};
