#pragma once

#include <string>

#include <entityx/entityx.h>

#include "Systems.h"

using namespace entityx;

class Scene : public EntityX {
public:
    Scene(std::string filename);

    void load(std::string filename);
    inline std::string getSceneName() { return scenename; }

private:
    std::string scenename;
    std::string filename;
};