#pragma once

#include <entityx/entityx.h>

using namespace entityx;

class ECS : public EntityX {
public:
    static ECS& instance() {
        static ECS *instance = new ECS();
        return *instance;
    }

    ECS(ECS const&) = delete;
    void operator=(ECS const&) = delete;

    void init();
    void update(TimeDelta dt);

private:
    ECS() {}
};